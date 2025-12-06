"""
Dataset Builder for KA-QFMS
Updated for latest Transformers API
"""
import os
import pickle
from pathlib import Path

import torch
from torch.utils.data import Dataset
from transformers import AutoTokenizer
import jsonlines


class OurDataset(Dataset):
    """
    Dataset for query-focused meeting summarization
    """
    
    def __init__(self, args, mode):
        """
        Initialize dataset
        
        Args:
            args: Arguments containing model paths and configurations
            mode: 'train', 'val', or 'test'
        """
        self.args = args
        self.mode = mode
        
        # Initialize tokenizer
        self.tokenizer = AutoTokenizer.from_pretrained(self.args.model)
        
        # Load original data
        original_data_path = Path('./data/qmsum/processed_data_no_sent_split') / f'{mode}.jsonl'
        if not original_data_path.exists():
            # Try alternative path (absolute from data_path)
            original_data_path = Path(self.args.data_path).parent / 'processed_data_no_sent_split' / f'{mode}.jsonl'
        
        if original_data_path.exists():
            self.src, self.tgt = self.file_reader(str(original_data_path))
        else:
            print(f"Warning: Original data file not found at {original_data_path}")
            print("Creating dummy data for structure validation...")
            self.src = ["Sample query </s> Sample source text"]
            self.tgt = ["Sample target summary"]
        
        # Load segmented data
        segmented_data_path = Path(self.args.data_path) / f'{self.mode}_12_dual_rank.pkl'
        if segmented_data_path.exists():
            with open(segmented_data_path, 'rb') as f:
                self.segmented_src = pickle.load(f)
        else:
            print(f"Warning: Segmented data file not found at {segmented_data_path}")
            print("You need to run data preprocessing first!")
            # Create dummy segmented data for testing
            self.segmented_src = [[src] for src in self.src]
        
        # Load knowledge data if knowledge-aware mode is enabled
        self.triples = None
        if self.args.knowledge_aware != '':
            keyphrases_path = Path(self.args.data_path) / f'{self.mode}_12_dual_rank_keyphrases.pkl'
            if keyphrases_path.exists():
                with open(keyphrases_path, 'rb') as f:
                    self.triples = pickle.load(f)
            else:
                print(f"Warning: Knowledge data file not found at {keyphrases_path}")
                # Create dummy triples
                self.triples = [["keyword1 </s> keyword2"] * len(seg) for seg in self.segmented_src]
        
        print(f"Loaded {mode} dataset with {len(self.segmented_src)} examples")
        if self.segmented_src:
            print(f"Segments per example: {[len(item) for item in self.segmented_src[:5]]}")

    def __len__(self):
        return len(self.segmented_src)

    def __getitem__(self, idx):
        if self.args.knowledge_aware != '':
            return self.segmented_src[idx], self.tgt[idx], self.triples[idx]
        else:
            return self.segmented_src[idx], self.tgt[idx]

    def file_reader(self, file_path):
        """Read JSONL file containing source and target pairs"""
        src = []
        tgt = []
        
        try:
            with jsonlines.open(file_path, 'r') as reader:
                for obj in reader:
                    src.append(obj['src'])
                    tgt.append(obj['tgt'])
        except Exception as e:
            print(f"Error reading file {file_path}: {e}")
            # Return empty lists on error
            return [], []
        
        return src, tgt

    def collate_fn(self, data):
        """
        Collate function for DataLoader
        Processes a batch of data into model inputs
        """
        # Extract source and target from batch
        # Limit to first 6 segments per document
        raw_src = [pair[0][:6] for pair in data]
        raw_tgt = [pair[1] for pair in data]
        
        # Extract queries (before </s> separator)
        queries = []
        for item in raw_src:
            if item and len(item) > 0:
                query = item[0].split('</s>')[0].strip()
                queries.append(query)
            else:
                queries.append("")
        
        # Extract documents without queries (after </s> separator)
        docs_without_query = []
        for item in raw_src:
            this_doc = []
            for para in item:
                parts = para.split('</s>')
                if len(parts) > 1:
                    this_doc.append(parts[-1].strip())
                else:
                    this_doc.append(para)
            docs_without_query.extend(this_doc)
        
        batch_size = len(raw_tgt)
        
        # Flatten source documents for encoding
        new_raw_src = []
        for item in raw_src:
            new_raw_src.extend(item)
        raw_src = new_raw_src
        
        # Tokenize source documents
        raw_ids = self.tokenizer(
            raw_src,
            add_special_tokens=True,
            padding='max_length',
            truncation=True,
            max_length=self.args.max_input_len,
            return_tensors='pt'
        )
        
        src_ids = raw_ids['input_ids']
        src_mask = raw_ids['attention_mask']
        
        # Tokenize target summaries
        raw_tgt_ids = self.tokenizer(
            raw_tgt,
            add_special_tokens=True,
            padding='max_length',
            truncation=True,
            max_length=self.args.max_output_len,
            return_tensors='pt'
        )['input_ids']
        
        # Prepare decoder input and labels based on model type
        if 'bart' in self.args.model.lower():
            from transformers.models.bart.modeling_bart import shift_tokens_right
            # BART uses pad_token_id=1, decoder_start_token_id=2
            tgt_ids = shift_tokens_right(
                raw_tgt_ids,
                pad_token_id=self.tokenizer.pad_token_id,
                decoder_start_token_id=self.tokenizer.bos_token_id or 2
            )
            # Set padding tokens in labels to -100 (ignored by loss)
            label_ids = raw_tgt_ids.clone()
            label_ids[label_ids == self.tokenizer.pad_token_id] = -100
            
        elif 'pegasus' in self.args.model.lower():
            from transformers.models.pegasus.modeling_pegasus import shift_tokens_right
            # Pegasus uses pad_token_id=0, decoder_start_token_id=0
            tgt_ids = shift_tokens_right(
                raw_tgt_ids,
                pad_token_id=self.tokenizer.pad_token_id,
                decoder_start_token_id=self.tokenizer.pad_token_id
            )
            label_ids = raw_tgt_ids.clone()
            label_ids[label_ids == self.tokenizer.pad_token_id] = -100
        else:
            tgt_ids = raw_tgt_ids
            label_ids = raw_tgt_ids
        
        # Knowledge-aware processing
        knowledge_inputs = None
        query_inputs = None
        doc_inputs = None
        
        if self.args.knowledge_aware != '':
            doc_with_query_triple = []
            raw_triples = [pair[2] for pair in data]
            
            for i, doc in enumerate(docs_without_query):
                # Get query for this batch (same for all segments)
                query = queries[0] if queries else ""
                
                # Get keywords for this segment
                batch_idx = i // len(raw_src) if len(raw_src) > 0 else 0
                seg_idx = i % len(raw_triples[batch_idx]) if batch_idx < len(raw_triples) else 0
                
                if batch_idx < len(raw_triples) and seg_idx < len(raw_triples[batch_idx]):
                    keywords_str = raw_triples[batch_idx][seg_idx]
                else:
                    keywords_str = ""
                
                # Process keywords
                keywords = keywords_str.split('</s>')
                keywords = list(set(keywords))
                keywords = [kw for kw in keywords[:10] if len(kw.strip()) > 3]
                keywords_text = " ".join(keywords)
                
                # Combine query, keywords, and document
                combined = f"{query} </s></s> {keywords_text} </s></s> {doc}"
                doc_with_query_triple.append(combined)
            
            # Tokenize combined inputs
            knowledge_inputs = self.tokenizer(
                doc_with_query_triple,
                add_special_tokens=True,
                padding='max_length',
                truncation=True,
                max_length=self.args.max_input_len,
                return_tensors='pt'
            )
            
            # Tokenize queries separately
            query_inputs = self.tokenizer(
                queries,
                add_special_tokens=True,
                padding='max_length',
                truncation=True,
                max_length=256,
                return_tensors='pt'
            )
            
            # Tokenize documents without query
            doc_inputs = self.tokenizer(
                docs_without_query,
                add_special_tokens=True,
                padding='max_length',
                truncation=True,
                max_length=self.args.max_input_len,
                return_tensors='pt'
            )
        
        return {
            'src_ids': [src_ids, knowledge_inputs, query_inputs, doc_inputs],
            'mask': src_mask,
            'decoder_ids': tgt_ids,
            'label_ids': label_ids,
            'labels': raw_tgt,
        }
