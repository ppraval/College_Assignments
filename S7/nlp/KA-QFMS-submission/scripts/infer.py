#!/usr/bin/env python3
"""
Lightweight inference script for KA-QFMS trained model.

Usage examples:
  python scripts/infer.py --checkpoint logs/bart_large_ka/checkpoints/best-epoch-01-32.900.ckpt \
    --text "Your long meeting text goes here..." \
    --output_file outputs/inference.txt

  python scripts/infer.py --checkpoint logs/bart_large_ka/checkpoints/best.ckpt \
    --input_file data/sample_inputs.jsonl --output_file outputs/inference.jsonl

The script chunks long input into segments compatible with the trained FiD model,
loads the Lightning checkpoint, runs generation on GPU (if available) and writes
the decoded summaries to the output file.
"""
import argparse
import json
import sys
from types import SimpleNamespace
from pathlib import Path
import torch

# Ensure src is on PYTHONPATH so imports like `from models.model import BaseModel` work
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'src'))

from models.model import BaseModel


def chunk_token_ids(tokenizer, text, max_len):
    """Tokenize and chunk text into pieces of at most max_len tokens.
    Returns a list of text segments (detokenized) to feed the model's segment encoder.
    """
    # Encode without special tokens so we can chunk then decode back to text segments
    ids = tokenizer.encode(text, add_special_tokens=False)
    if len(ids) <= max_len:
        return [text]

    segments = []
    for i in range(0, len(ids), max_len):
        chunk = ids[i:i+max_len]
        seg_text = tokenizer.decode(chunk, skip_special_tokens=True, clean_up_tokenization_spaces=True)
        segments.append(seg_text)
    return segments


def prepare_inputs(tokenizer, segments, max_input_len):
    """Tokenize a list of segment texts and return input_ids tensor and attention_mask tensor.
    The returned tensors are shaped (num_segments, seq_len).
    """
    enc = tokenizer(
        segments,
        add_special_tokens=True,
        padding='max_length',
        truncation=True,
        max_length=max_input_len,
        return_tensors='pt'
    )
    return enc['input_ids'], enc['attention_mask']


def load_model_from_checkpoint(checkpoint_path, args_namespace, device):
    # Load LightningModule from checkpoint; pass args so __init__ receives expected namespace
    print(f"Loading checkpoint: {checkpoint_path}")
    model = BaseModel.load_from_checkpoint(str(checkpoint_path), args=args_namespace, map_location=device)
    model.model.to(device)
    model.model.eval()
    return model


def infer_single(model, tokenizer, text, args):
    # Chunk text into segments
    segments = chunk_token_ids(tokenizer, text, args.max_input_len - 2)
    # Limit to number_of_segment (same behaviour as dataset)
    segments = segments[: args.number_of_segment]

    src_ids, src_mask = prepare_inputs(tokenizer, segments, args.max_input_len)

    device = torch.device(args.device)
    src_ids = src_ids.to(device)
    src_mask = src_mask.to(device)

    gen_kwargs = dict(
        input_ids=[src_ids],
        attention_mask=src_mask,
        num_beams=args.n_beams,
        max_length=args.max_output_len,
        min_length=args.min_output_len,
        no_repeat_ngram_size=args.no_repeat_ngram_size,
        length_penalty=args.length_penalty,
        early_stopping=True,
    )

    with torch.no_grad():
        summary_ids = model.model.generate(**gen_kwargs)

    # summary_ids shape: (batch_size, seq_len)
    decoded = [tokenizer.decode(g, skip_special_tokens=True, clean_up_tokenization_spaces=False) for g in summary_ids]
    return decoded


def main():
    parser = argparse.ArgumentParser(description='Inference script for KA-QFMS')
    parser.add_argument('--checkpoint', required=True, help='Path to Lightning checkpoint (.ckpt)')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--text', help='Raw input text to summarize')
    group.add_argument('--input_file', help='JSONL file with one object per line containing field "src" (raw text)')
    parser.add_argument('--output_file', default='outputs/inference.txt', help='Where to write decoded summaries')
    parser.add_argument('--device', default='cuda' if torch.cuda.is_available() else 'cpu')

    # Generation / model settings (default match training defaults)
    parser.add_argument('--model_name', default='facebook/bart-large')
    parser.add_argument('--max_input_len', type=int, default=512)
    parser.add_argument('--number_of_segment', type=int, default=6)
    parser.add_argument('--n_beams', type=int, default=5)
    parser.add_argument('--max_output_len', type=int, default=256)
    parser.add_argument('--min_output_len', type=int, default=64)
    parser.add_argument('--no_repeat_ngram_size', type=int, default=3)
    parser.add_argument('--length_penalty', type=float, default=0.5)

    args = parser.parse_args()

    # Build a minimal args namespace expected by BaseModel.__init__
    args_ns = argparse.Namespace(
        model=args.model_name,
        n_beams=args.n_beams,
        max_output_len=args.max_output_len,
        min_output_len=args.min_output_len,
        no_repeat_ngram_size=args.no_repeat_ngram_size,
        length_penalty=args.length_penalty,
        number_of_segment=args.number_of_segment,
        max_input_len=args.max_input_len,
        knowledge_aware='',
        val_save_file='',
        test_save_file='',
        data_path='./qmsum_data/512',
        # filler fields used elsewhere
        checkpoint='None',
        gpus='0'
    )

    device = torch.device(args.device)

    model = load_model_from_checkpoint(args.checkpoint, args_ns, device)
    tokenizer = model.tokenizer

    out_path = Path(args.output_file)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    results = []
    if args.text:
        decoded = infer_single(model, tokenizer, args.text, args)
        results = decoded
    else:
        # Read JSONL file, expect field 'src'
        import jsonlines
        with jsonlines.open(args.input_file, 'r') as reader:
            for obj in reader:
                text = obj.get('src') or obj.get('text') or ''
                decoded = infer_single(model, tokenizer, text, args)
                # store one decoded summary per input (take first if many)
                results.append(decoded[0] if decoded else "")

    # Write outputs
    if args.output_file.endswith('.jsonl'):
        import jsonlines
        with jsonlines.open(args.output_file, 'w') as writer:
            if args.text:
                writer.write({'summary': results[0]})
            else:
                for s in results:
                    writer.write({'summary': s})
    else:
        with open(args.output_file, 'w', encoding='utf-8') as f:
            if args.text:
                f.write(results[0] + '\n')
            else:
                for s in results:
                    f.write(s + '\n')

    print(f"Wrote {len(results)} summaries to {args.output_file}")


if __name__ == '__main__':
    main()
