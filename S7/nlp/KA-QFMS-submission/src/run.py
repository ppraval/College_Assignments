"""
KA-QFMS Training Script
Updated for PyTorch Lightning 2.x and latest dependencies
"""
import argparse
import os
import sys
from pathlib import Path

import torch
from torch.utils.data import DataLoader
from pytorch_lightning import Trainer, seed_everything
from pytorch_lightning.callbacks import ModelCheckpoint
from pytorch_lightning.loggers import TensorBoardLogger

from data_builder import OurDataset
from models.model import BaseModel

# Disable tokenizers parallelism warning
os.environ["TOKENIZERS_PARALLELISM"] = "false"

# Enable TF32 for A100 GPUs (better performance with minimal precision loss)
# Use new API only to avoid conflicts
if torch.cuda.is_available():
    torch.set_float32_matmul_precision('medium')  # or 'high' for even faster computation


class Logger(object):
    """Custom logger that writes to both terminal and file"""
    def __init__(self, filename="Default.log"):
        self.terminal = sys.stdout
        self.log = open(filename, "a")
    
    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)
    
    def flush(self):
        self.terminal.flush()
        self.log.flush()


def main():
    parser = argparse.ArgumentParser(description="KA-QFMS Training and Evaluation")
    
    # Data paths
    parser.add_argument('-data_path', default='', type=str, 
                        help='Path to processed data')
    parser.add_argument('-val_save_file', default='', type=str,
                        help='Path to save validation outputs')
    parser.add_argument('-test_save_file', default='', type=str,
                        help='Path to save test outputs')

    # Model settings
    parser.add_argument('-model', default='facebook/bart-large', type=str,
                        help='Model name or path (e.g., facebook/bart-large, google/pegasus-large)')
    parser.add_argument('-checkpoint', default='None', type=str,
                        help='Path to checkpoint to resume from')
    parser.add_argument('-segment_interaction', default='no', type=str,
                        help='Whether to use segment interaction')
    parser.add_argument('-knowledge_aware', default='', type=str,
                        help='Knowledge awareness mode: multihead, concat, or empty string')

    # Training settings
    parser.add_argument('-log_name', default='BART_large', type=str,
                        help='Name for logging directory')
    parser.add_argument('-gpus', default='0', type=str,
                        help='GPU devices to use (comma-separated)')
    parser.add_argument('-batch_size', type=int, default=4,
                        help='Batch size per GPU')
    parser.add_argument('-learning_rate', default=3e-5, type=float,
                        help='Learning rate')
    parser.add_argument('-adafactor', action='store_true',
                        help='Use Adafactor optimizer instead of Adam')
    parser.add_argument('-num_epochs', type=int, default=100,
                        help='Maximum number of epochs')
    parser.add_argument('-warmup', type=int, default=20,
                        help='Number of warmup steps')
    parser.add_argument('-label_smoothing', type=float, default=0.1,
                        help='Label smoothing factor')
    parser.add_argument('-grad_accum', type=int, default=10,
                        help='Gradient accumulation steps')
    parser.add_argument('-random_seed', type=int, default=0,
                        help='Random seed for reproducibility')
    parser.add_argument('-do_train', action='store_true',
                        help='Run training')
    parser.add_argument('-do_test', action='store_true',
                        help='Run testing')
    parser.add_argument('-limit_val_batches', default=1.0, type=float,
                        help='Fraction of validation batches to use')
    parser.add_argument('-val_check_interval', type=float, default=1.0,
                        help='How often to check validation within an epoch')

    # Data settings
    parser.add_argument('-max_input_len', type=int, default=512,
                        help='Maximum input sequence length')
    parser.add_argument('-number_of_segment', type=int, default=16,
                        help='Number of segments per document')
    parser.add_argument('-max_output_len', type=int, default=256,
                        help='Maximum output sequence length')
    parser.add_argument('-min_output_len', type=int, default=64,
                        help='Minimum output sequence length')
    parser.add_argument('-n_beams', type=int, default=5,
                        help='Number of beams for beam search')
    parser.add_argument('-no_repeat_ngram_size', type=int, default=3,
                        help='Size of n-grams that cannot be repeated')
    parser.add_argument('-length_penalty', type=float, default=0.5,
                        help='Length penalty for beam search')
    
    # Training strategy
    parser.add_argument('-strategy', type=str, default='auto',
                        help='Training strategy: auto, ddp, deepspeed, etc.')
    parser.add_argument('-precision', type=str, default='32',
                        help='Training precision: 32, 16, bf16, etc.')

    args = parser.parse_args()

    # Set random seed for reproducibility
    seed_everything(args.random_seed, workers=True)

    # Create terminal output directory
    terminal_output_dir = Path('terminal_output')
    terminal_output_dir.mkdir(exist_ok=True)
    
    # Set up logging
    log_file = terminal_output_dir / f'{args.log_name.split("/")[-1]}_{args.random_seed}.log'
    sys.stdout = Logger(str(log_file))
    print(f"Arguments: {args}")

    # Set up TensorBoard logger
    logger = TensorBoardLogger(save_dir=args.log_name, name="", version="")

    # Checkpoint callback - saves best model based on validation ROUGE-1 F1
    checkpoint_dir = Path(args.log_name) / 'checkpoints'
    checkpoint_dir.mkdir(parents=True, exist_ok=True)
    
    checkpoint_callback = ModelCheckpoint(
        dirpath=str(checkpoint_dir),
        monitor='validation_Rouge/rouge1_F1',
        save_last=True,
        save_top_k=1,
        mode='max',
        filename='best-{epoch:02d}-{validation_Rouge/rouge1_F1:.4f}'
    )

    # Determine checkpoint path
    resume_checkpoint = None if args.checkpoint == 'None' else args.checkpoint

    # Parse GPU devices
    if args.gpus == '-1':
        devices = 'auto'
    else:
        devices = [int(x) for x in args.gpus.split(',')]

    # Determine strategy
    if args.strategy == 'deepspeed':
        strategy = 'deepspeed_stage_2'
    else:
        strategy = args.strategy

    # Create Trainer with updated API (PyTorch Lightning 2.x)
    trainer = Trainer(
        accelerator='gpu' if torch.cuda.is_available() else 'cpu',
        devices=devices,
        strategy=strategy,
        precision=args.precision,
        max_epochs=args.num_epochs,
        accumulate_grad_batches=args.grad_accum,
        gradient_clip_val=1.0,
        val_check_interval=args.val_check_interval,
        limit_val_batches=args.limit_val_batches,
        logger=logger,
        callbacks=[checkpoint_callback],
        log_every_n_steps=1,
        num_sanity_val_steps=4,
        deterministic=True,
        enable_progress_bar=True,
    )

    # Create datasets
    print("Loading datasets...")
    train_set = OurDataset(args, 'train')
    val_set = OurDataset(args, 'val')
    test_set = OurDataset(args, 'test')

    # Create dataloaders
    train_loader = DataLoader(
        dataset=train_set,
        batch_size=args.batch_size,
        num_workers=4,
        shuffle=True,
        collate_fn=train_set.collate_fn,
        pin_memory=True
    )
    
    val_loader = DataLoader(
        dataset=val_set,
        batch_size=args.batch_size,
        num_workers=4,
        shuffle=False,
        collate_fn=val_set.collate_fn,
        pin_memory=True
    )
    
    test_loader = DataLoader(
        dataset=test_set,
        batch_size=args.batch_size,
        num_workers=4,
        shuffle=False,
        collate_fn=test_set.collate_fn,
        pin_memory=True
    )

    # Initialize model
    print("Initializing model...")
    model = BaseModel(args)

    # Train the model
    if args.do_train:
        print("Starting training...")
        trainer.fit(
            model,
            train_dataloaders=train_loader,
            val_dataloaders=val_loader,
            ckpt_path=resume_checkpoint
        )

    # Test the model
    if args.do_test:
        print("Starting testing...")
        test_checkpoint = args.checkpoint if args.checkpoint != 'None' else 'best'
        trainer.test(
            model=model,
            dataloaders=test_loader,
            ckpt_path=test_checkpoint
        )

    print("Done!")


if __name__ == '__main__':
    main()
