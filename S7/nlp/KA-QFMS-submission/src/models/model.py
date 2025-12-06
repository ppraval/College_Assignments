"""
Base Model for KA-QFMS
Updated for PyTorch Lightning 2.x and latest Transformers
"""
import torch
import torch.nn as nn
import torch.nn.functional as F
import pytorch_lightning as pl
from transformers import AutoTokenizer, get_linear_schedule_with_warmup
from rouge_score import rouge_scorer
import numpy as np

from models.fidbart import BartForMultiConditionalGeneration
from models.fidpegasus import PegasusForMultiConditionalGeneration


class BaseModel(pl.LightningModule):
    """
    Base PyTorch Lightning module for query-focused meeting summarization
    """
    
    def __init__(self, args):
        super().__init__()
        self.save_hyperparameters(args)
        self.args = args
        
        # Initialize model based on type
        if 'bart' in self.args.model.lower():
            self.model = BartForMultiConditionalGeneration.from_pretrained(
                self.args.model, 
                args=self.args
            )
        elif 'pegasus' in self.args.model.lower():
            self.model = PegasusForMultiConditionalGeneration.from_pretrained(
                self.args.model,
                args=self.args
            )
        else:
            raise ValueError(f"Unsupported model type: {self.args.model}")
        
        # Initialize tokenizer
        self.tokenizer = AutoTokenizer.from_pretrained(self.args.model)
        
        # Initialize ROUGE scorer
        self.rouge = rouge_scorer.RougeScorer(
            ['rouge1', 'rouge2', 'rougeL'],
            use_stemmer=True
        )
        
        # Store for validation/test outputs
        self.validation_step_outputs = []
        self.test_step_outputs = []

    def forward(self, input_ids, attention_mask, decoder_input_ids, labels):
        """Forward pass through the model"""
        outputs = self.model(
            input_ids=input_ids,
            attention_mask=attention_mask,
            decoder_input_ids=decoder_input_ids,
            labels=labels
        )
        return outputs.loss

    def training_step(self, batch, batch_idx):
        """Training step"""
        loss = self(
            input_ids=batch['src_ids'],
            attention_mask=batch['mask'],
            decoder_input_ids=batch['decoder_ids'],
            labels=batch['label_ids']
        )
        
        # Log metrics
        self.log('train/loss', loss, on_step=True, on_epoch=True, prog_bar=True, sync_dist=True)
        
        # Log learning rate
        lr = self.trainer.optimizers[0].param_groups[0]['lr']
        self.log('train/lr', lr, on_step=True, on_epoch=True, prog_bar=True)
        
        return loss

    def validation_step(self, batch, batch_idx):
        """Validation step"""
        # Generate summaries
        summary_ids = self.model.generate(
            input_ids=batch['src_ids'],
            attention_mask=batch['mask'],
            num_beams=self.args.n_beams,
            max_length=self.args.max_output_len,
            min_length=self.args.min_output_len,
            no_repeat_ngram_size=self.args.no_repeat_ngram_size,
            length_penalty=self.args.length_penalty,
            early_stopping=True
        )
        
        # Store outputs for epoch-end processing
        self.validation_step_outputs.append({
            'summary_ids': summary_ids,
            'labels': batch['labels']
        })
        
        return summary_ids

    def on_validation_epoch_end(self):
        """Process validation outputs at epoch end"""
        summaries = []
        references = []
        
        # Decode all summaries
        for output in self.validation_step_outputs:
            summary_ids = output['summary_ids']
            decoded_summaries = [
                self.tokenizer.decode(
                    [i for i in g if i != -100],
                    skip_special_tokens=True,
                    clean_up_tokenization_spaces=False
                )
                for g in summary_ids
            ]
            summaries.extend(decoded_summaries)
            references.extend(output['labels'])
        
        # Calculate ROUGE scores
        if summaries and references:
            r1_f1, r2_f1, rl_f1 = self.calculate_rouge(summaries, references)
            
            # Log ROUGE scores
            self.log('validation_Rouge/rouge1_F1', r1_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            self.log('validation_Rouge/rouge2_F1', r2_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            self.log('validation_Rouge/rougeL_F1', rl_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            
            # Save summaries to file
            if self.args.val_save_file:
                self.save_txt(self.args.val_save_file, summaries)
        
        # Clear outputs
        self.validation_step_outputs.clear()

    def test_step(self, batch, batch_idx):
        """Test step"""
        # Generate summaries
        summary_ids = self.model.generate(
            input_ids=batch['src_ids'],
            attention_mask=batch['mask'],
            num_beams=self.args.n_beams,
            max_length=self.args.max_output_len,
            min_length=self.args.min_output_len,
            no_repeat_ngram_size=self.args.no_repeat_ngram_size,
            length_penalty=self.args.length_penalty,
            early_stopping=True
        )
        
        # Store outputs for epoch-end processing
        self.test_step_outputs.append({
            'summary_ids': summary_ids,
            'labels': batch['labels']
        })
        
        return summary_ids

    def on_test_epoch_end(self):
        """Process test outputs at epoch end"""
        summaries = []
        references = []
        
        # Decode all summaries
        for output in self.test_step_outputs:
            summary_ids = output['summary_ids']
            decoded_summaries = [
                self.tokenizer.decode(
                    [i for i in g if i != -100],
                    skip_special_tokens=True,
                    clean_up_tokenization_spaces=False
                )
                for g in summary_ids
            ]
            summaries.extend(decoded_summaries)
            references.extend(output['labels'])
        
        # Calculate ROUGE scores
        if summaries and references:
            r1_f1, r2_f1, rl_f1 = self.calculate_rouge(summaries, references)
            
            # Log ROUGE scores
            self.log('test_Rouge/rouge1_F1', r1_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            self.log('test_Rouge/rouge2_F1', r2_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            self.log('test_Rouge/rougeL_F1', rl_f1, on_epoch=True, prog_bar=True, sync_dist=True)
            
            # Save summaries to file
            if self.args.test_save_file:
                self.save_txt(self.args.test_save_file, summaries)
        
        # Clear outputs
        self.test_step_outputs.clear()

    def calculate_rouge(self, summaries, references):
        """Calculate ROUGE scores"""
        rouge1_scores = []
        rouge2_scores = []
        rougeL_scores = []
        
        for summary, reference in zip(summaries, references):
            scores = self.rouge.score(summary, reference)
            rouge1_scores.append(scores['rouge1'].fmeasure)
            rouge2_scores.append(scores['rouge2'].fmeasure)
            rougeL_scores.append(scores['rougeL'].fmeasure)
        
        r1_f1 = np.mean(rouge1_scores) * 100
        r2_f1 = np.mean(rouge2_scores) * 100
        rl_f1 = np.mean(rougeL_scores) * 100
        
        return r1_f1, r2_f1, rl_f1

    def save_txt(self, file_name, list_data):
        """Save text data to file"""
        with open(file_name, 'w', encoding='utf-8') as f:
            for item in list_data:
                f.write(item + '\n')

    def configure_optimizers(self):
        """Configure optimizers and learning rate schedulers"""
        if self.args.adafactor:
            from transformers import Adafactor
            optimizer = Adafactor(
                self.model.parameters(),
                lr=self.args.learning_rate,
                eps=(1e-30, 1e-3),
                clip_threshold=1.0,
                decay_rate=-0.8,
                beta1=None,
                weight_decay=0.0,
                relative_step=False,
                scale_parameter=False,
                warmup_init=False
            )
        else:
            optimizer = torch.optim.AdamW(
                self.model.parameters(),
                lr=self.args.learning_rate,
                eps=1e-8,
                weight_decay=0.01
            )
        
        # Calculate total training steps
        if hasattr(self.trainer, 'estimated_stepping_batches'):
            total_steps = self.trainer.estimated_stepping_batches
        else:
            # Fallback calculation
            total_steps = (
                len(self.trainer.datamodule.train_dataloader()) 
                * self.args.num_epochs 
                // self.args.grad_accum
            ) if hasattr(self.trainer, 'datamodule') else 1000
        
        # Create learning rate scheduler with warmup
        scheduler = get_linear_schedule_with_warmup(
            optimizer,
            num_warmup_steps=self.args.warmup,
            num_training_steps=total_steps
        )
        
        scheduler_config = {
            'scheduler': scheduler,
            'interval': 'step',
            'frequency': 1,
            'name': 'learning_rate'
        }
        
        return {
            'optimizer': optimizer,
            'lr_scheduler': scheduler_config
        }
