# KA-QFMS: Knowledge-Aware Query-Focused Meeting Summarization

Complete guide for training and inference with the KA-QFMS model on NVIDIA A100 GPUs.

## Table of Contents
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Data Preparation](#data-preparation)
- [Training](#training)
- [Inference](#inference)
- [Model Checkpoints](#model-checkpoints)
- [Troubleshooting](#troubleshooting)
- [Performance Metrics](#performance-metrics)

---

## Overview

KA-QFMS is a query-focused meeting summarization system based on Fusion-in-Decoder (FiD) architecture with BART-large. The model generates summaries that specifically answer queries about meeting content.

**Key Features:**
- Query-focused summarization for meeting transcripts
- Multi-segment document encoding using FiD approach
- Supports long documents via intelligent chunking (up to 6 segments × 512 tokens)
- Optimized for NVIDIA A100 GPUs with TF32 precision
- PyTorch Lightning 2.x framework

**Model Architecture:**
- Base Model: `facebook/bart-large` (406M parameters)
- Encoder: Multi-segment fusion encoder (processes up to 6 document segments)
- Decoder: Standard BART decoder with beam search generation

---

## Hardware Requirements

**Recommended:**
- GPU: NVIDIA A100 80GB PCIe (or similar)
- RAM: 32GB+ system memory
- Storage: 50GB+ free space
- CUDA: 12.0+ compatible

**Minimum:**
- GPU: NVIDIA GPU with 16GB+ VRAM
- RAM: 16GB+ system memory
- CUDA: 11.0+ compatible

---

## Installation

### 1. Clone the Repository
```bash
cd /path/to/your/workspace
# Repository should already be present
```

### 2. Create Virtual Environment
```bash
python3.10 -m venv .venv
source .venv/bin/activate
```

### 3. Install Dependencies
```bash
pip install --upgrade pip
pip install -r requirements.txt
```

**Key Dependencies:**
- PyTorch 2.5.0+ with CUDA support
- PyTorch Lightning 2.x
- Transformers 4.46.0+
- ROUGE-score for evaluation
- jsonlines for data I/O

### 4. Verify Installation
```bash
python test_installation.py
```

Expected output:
```
✓ PyTorch
✓ PyTorch Lightning
✓ Transformers
✓ CUDA is available
  - GPU 0: NVIDIA A100 80GB PCIe
✓ All tests passed!
```

---

## Project Structure

```
KA-QFMS-updated/
├── src/
│   ├── run.py                      # Main training script
│   ├── data_builder.py             # Dataset and data loading
│   └── models/
│       ├── model.py                # PyTorch Lightning module
│       ├── fidbart.py              # FiD-BART implementation
│       └── fidpegasus.py           # FiD-Pegasus implementation
├── scripts/
│   ├── QMSum.sh                    # Training launch script
│   ├── infer.py                    # Inference script
│   └── test.sh                     # Testing script
├── qmsum_data/
│   ├── 512/                        # Preprocessed data (512 token chunks)
│   │   ├── train_12_dual_rank.pkl
│   │   ├── val_12_dual_rank.pkl
│   │   └── test_12_dual_rank.pkl
│   └── processed_data_no_sent_split/
│       ├── train.jsonl
│       ├── val.jsonl
│       └── test.jsonl
├── data/
│   └── my_inputs.jsonl             # Sample inference inputs
├── logs/
│   └── bart_large_ka/
│       ├── checkpoints/            # Model checkpoints
│       └── events.out.tfevents.*   # TensorBoard logs
├── outputs/                        # Generated summaries
├── requirements.txt                # Python dependencies
└── README_COMPLETE.md             # This file
```

---

## Data Preparation

### Data Format

**Training Data (JSONL):**
```json
{"src": "Query: What was discussed? Meeting: Speaker A said...", "tgt": "Summary text"}
```

**Inference Data (JSONL):**
```json
{"src": "Query: Your question? Meeting transcript: Full meeting text..."}
```

### Preprocessing

The QMSum dataset should be preprocessed into:
1. **Original JSONL files** (`processed_data_no_sent_split/*.jsonl`)
   - Contains query + meeting transcript pairs
   - One example per line

2. **Segmented pickle files** (`512/*_12_dual_rank.pkl`)
   - Pre-chunked into 512-token segments
   - Ranked by relevance to query
   - Maximum 12 segments per document

### Dataset Statistics

- **Training set:** 1,257 meeting summaries
- **Validation set:** 272 meeting summaries
- **Test set:** 281 meeting summaries
- **Average segments per document:** 12
- **Token limit per segment:** 512

---

## Training

### Quick Start

```bash
# Activate environment
source .venv/bin/activate

# Start training (runs in background)
nohup bash scripts/QMSum.sh > QMSum.log 2>&1 &

# Monitor training
tail -f QMSum.log
```

### Training Script (QMSum.sh)

```bash
#!/bin/bash

python3 ./src/run.py \
    -data_path './qmsum_data/512' \
    -val_save_file './outputs/val_out.txt' \
    -test_save_file './outputs/test_results.txt' \
    -model 'facebook/bart-large' \
    -checkpoint 'None' \
    -segment_interaction 'no' \
    -knowledge_aware '' \
    -log_name 'logs/bart_large_ka' \
    -gpus '0' \
    -batch_size 1 \
    -learning_rate 5e-6 \
    -num_epochs 10 \
    -warmup 500 \
    -label_smoothing 0.1 \
    -grad_accum 4 \
    -random_seed 42 \
    -do_train \
    -limit_val_batches 1.0 \
    -val_check_interval 1.0 \
    -max_input_len 512 \
    -number_of_segment 6 \
    -max_output_len 256 \
    -min_output_len 64 \
    -n_beams 5 \
    -no_repeat_ngram_size 3 \
    -length_penalty 0.5 \
    -strategy 'auto' \
    -precision '16'
```

### Key Training Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| `batch_size` | 1 | Batch size per GPU |
| `grad_accum` | 4 | Gradient accumulation steps (effective batch = 4) |
| `learning_rate` | 5e-6 | Learning rate |
| `warmup` | 500 | Linear warmup steps |
| `num_epochs` | 10 | Maximum training epochs |
| `precision` | 16 | Mixed precision training (FP16) |
| `max_input_len` | 512 | Max tokens per segment |
| `number_of_segment` | 6 | Max segments per document |
| `max_output_len` | 256 | Max summary length |

### Training Optimizations (A100)

The code automatically enables:
- **TF32 precision** for 3× faster training on A100
- **Mixed precision (FP16)** training
- **Gradient accumulation** for larger effective batch sizes
- **Automatic checkpoint saving** based on validation ROUGE-1 F1

### Monitor Training Progress

**Option 1: Log file**
```bash
tail -f QMSum.log
```

**Option 2: TensorBoard**
```bash
tensorboard --logdir=logs/bart_large_ka
# Open browser to http://localhost:6006
```

### Training Metrics

The model logs:
- **Training loss** per step and epoch
- **Learning rate** schedule
- **Validation ROUGE scores** (ROUGE-1, ROUGE-2, ROUGE-L)
- **Training speed** (~5-6 iterations/second on A100)

**Example validation results after Epoch 1:**
- ROUGE-1 F1: **32.90**
- ROUGE-2 F1: **9.12**
- ROUGE-L F1: **20.80**

---

## Inference

### Quick Start

**Single text input:**
```bash
source .venv/bin/activate

python3 scripts/infer.py \
  --checkpoint logs/bart_large_ka/checkpoints/last.ckpt \
  --text "Query: What were the main decisions? Meeting: Speaker A said..." \
  --output_file outputs/summary.txt
```

**Batch inference from JSONL:**
```bash
python3 scripts/infer.py \
  --checkpoint logs/bart_large_ka/checkpoints/last.ckpt \
  --input_file data/my_inputs.jsonl \
  --output_file outputs/results.jsonl
```

### Inference Script Options

```bash
python3 scripts/infer.py \
  --checkpoint <path_to_checkpoint.ckpt>    # Required: model checkpoint
  --text "Your text..."                      # Option 1: direct text input
  --input_file <input.jsonl>                 # Option 2: batch from file
  --output_file <output.txt>                 # Output file (default: outputs/inference.txt)
  --device cuda                              # Device: cuda or cpu (auto-detected)
  --n_beams 5                                # Beam search beams (default: 5)
  --max_output_len 256                       # Max summary length (default: 256)
  --min_output_len 64                        # Min summary length (default: 64)
  --no_repeat_ngram_size 3                   # Prevent n-gram repetition (default: 3)
  --length_penalty 0.5                       # Length penalty (default: 0.5)
  --max_input_len 512                        # Max tokens per segment (default: 512)
  --number_of_segment 6                      # Max segments to use (default: 6)
```

### Input Format

**For `--text` option:**
- Raw text containing query and meeting transcript
- The model will automatically chunk long inputs

**For `--input_file` option (JSONL):**
```json
{"src": "Query: What was discussed? Meeting: Full transcript here..."}
{"src": "Query: Another question? Meeting: Another transcript..."}
```

### Output Format

**Plain text output (`.txt`):**
```
Summary for input 1.
Summary for input 2.
...
```

**JSONL output (`.jsonl`):**
```json
{"summary": "Summary for input 1."}
{"summary": "Summary for input 2."}
```

### Sample Inference Examples

The repository includes sample meeting transcripts in `data/my_inputs.jsonl`:

1. **Project Timeline Discussion**
   - Query about main decisions on project timeline
   - 4-month accelerated timeline with additional resources

2. **Database Performance Issues**
   - Technical issues and proposed solutions
   - Indexing improvements and query optimization

3. **Marketing Budget Allocation**
   - Budget distribution across channels
   - $500K total allocation breakdown

Run inference on samples:
```bash
python3 scripts/infer.py \
  --checkpoint logs/bart_large_ka/checkpoints/last.ckpt \
  --input_file data/my_inputs.jsonl \
  --output_file outputs/sample_results.jsonl
```

---

## Model Checkpoints

### Checkpoint Location

Checkpoints are saved in: `logs/bart_large_ka/checkpoints/`

**Types of checkpoints:**
- `best-epoch=XX-validation_Rouge/...` - Best model by validation ROUGE-1 F1
- `last.ckpt` - Most recent checkpoint (saved every epoch)

### Loading Checkpoints

**For inference:**
```bash
python3 scripts/infer.py --checkpoint logs/bart_large_ka/checkpoints/last.ckpt ...
```

**For resuming training:**
```bash
# Edit scripts/QMSum.sh:
-checkpoint 'logs/bart_large_ka/checkpoints/last.ckpt'
```

### Checkpoint Contents

Each checkpoint includes:
- Model weights (406M parameters)
- Optimizer state
- Learning rate scheduler state
- Hyperparameters
- Training step information

**Size:** ~4.6GB per checkpoint

---

## Troubleshooting

### Common Issues and Solutions

#### 1. CUDA Out of Memory
**Error:** `RuntimeError: CUDA out of memory`

**Solutions:**
- Reduce `batch_size` to 1
- Reduce `number_of_segment` from 6 to 4
- Reduce `max_input_len` from 512 to 384
- Increase `grad_accum` (keeps effective batch size same)

```bash
# In QMSum.sh:
-batch_size 1 \
-number_of_segment 4 \
-grad_accum 8
```

#### 2. Cache Position Error
**Error:** `got an unexpected keyword argument 'cache_position'`

**Solution:** Already fixed in `src/models/fidbart.py`
- The forward method now accepts `**kwargs` to handle new transformer parameters

#### 3. TF32 API Conflict
**Error:** `RuntimeError: PyTorch is checking the matmul precision...`

**Solution:** Already fixed in `src/run.py`
- Uses only the new TF32 API: `torch.set_float32_matmul_precision('medium')`

#### 4. Checkpoint Save Error
**Error:** Checkpoint directory not found during saving

**Solution:** Already fixed in `src/run.py`
- Checkpoint directory is now created automatically with proper `dirpath`

#### 5. Module Import Error (Inference)
**Error:** `ModuleNotFoundError: No module named 'models'`

**Solution:** Already fixed in `scripts/infer.py`
- Script now adds `src/` to Python path automatically

#### 6. Slow Training Speed
**Solutions:**
- Ensure TF32 is enabled (automatic on A100)
- Use mixed precision: `-precision '16-mixed'`
- Increase `grad_accum` to reduce optimizer steps
- Check GPU utilization: `nvidia-smi`

#### 7. Data Files Not Found
**Error:** `Warning: Segmented data file not found`

**Check:**
```bash
ls qmsum_data/512/
ls qmsum_data/processed_data_no_sent_split/
```

**If missing:** The code will create dummy data for testing, but you need actual preprocessed data for real training.

---

## Performance Metrics

### Training Performance (A100 80GB)

- **Training speed:** ~5.5 iterations/second
- **GPU memory usage:** ~40-45GB (with batch_size=1, 6 segments)
- **Time per epoch:** ~16 minutes (1,257 training examples)
- **Total training time:** ~2.7 hours (10 epochs)

### Generation Performance

- **Inference speed:** ~2-3 seconds per summary (beam_size=5)
- **GPU memory usage:** ~15-20GB during generation
- **CPU inference:** Supported but ~10× slower

### Model Quality (After 1 Epoch)

| Metric | Score |
|--------|-------|
| ROUGE-1 F1 | 32.90 |
| ROUGE-2 F1 | 9.12 |
| ROUGE-L F1 | 20.80 |

*Note: Scores typically improve significantly after full training (10 epochs)*

---

## Advanced Usage

### Multi-GPU Training

To use multiple GPUs:
```bash
# In QMSum.sh:
-gpus '0,1,2,3' \
-strategy 'ddp'
```

### Using Different Base Models

```bash
# For Pegasus:
-model 'google/pegasus-large'

# For other BART variants:
-model 'facebook/bart-base'
```

### Knowledge-Aware Mode

To enable knowledge-aware features:
```bash
# In QMSum.sh:
-knowledge_aware 'multihead'
```

Requires additional preprocessed knowledge data files (`*_keyphrases.pkl`).

### Custom Generation Parameters

Experiment with different generation settings:
```bash
# More diverse outputs:
python3 scripts/infer.py \
  --n_beams 10 \
  --length_penalty 1.0 \
  --max_output_len 512

# Shorter, more focused summaries:
python3 scripts/infer.py \
  --n_beams 3 \
  --length_penalty 0.3 \
  --max_output_len 128 \
  --min_output_len 32
```

---

## Citation

If you use this code or models, please cite:

```bibtex
@article{qmsum2021,
  title={QMSum: A New Benchmark for Query-based Multi-domain Meeting Summarization},
  author={Zhong, Ming and Yin, Da and Yu, Tao and Zaidi, Ahmad and Mutuma, Mutethia and Jha, Rahul and Awadallah, Ahmed Hassan and Celikyilmaz, Asli and Liu, Yang and Qiu, Xipeng and Radev, Dragomir},
  journal={NAACL},
  year={2021}
}
```

---

## Contact & Support

For questions or issues:
1. Check the [Troubleshooting](#troubleshooting) section
2. Review training logs: `tail -f QMSum.log`
3. Check TensorBoard: `tensorboard --logdir=logs/`
4. Review checkpoint files: `ls -lh logs/bart_large_ka/checkpoints/`

---

## License

This project uses:
- BART model from Hugging Face (Apache 2.0)
- QMSum dataset (Check original dataset license)
- PyTorch and PyTorch Lightning (BSD-style license)

---

## Changelog

### Recent Updates
- ✅ Fixed `cache_position` compatibility with latest Transformers
- ✅ Added TF32 optimization for A100 GPUs
- ✅ Fixed checkpoint directory creation
- ✅ Added comprehensive inference script
- ✅ Improved error handling and logging
- ✅ Added sample meeting data for testing

---

**Last Updated:** November 5, 2025
**Tested With:**
- Python 3.10
- PyTorch 2.5.0
- Transformers 4.46.0
- PyTorch Lightning 2.4.0
- CUDA 12.6
- NVIDIA A100 80GB PCIe
