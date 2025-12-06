#!/bin/bash
# Training script for KA-QFMS
# Updated for new codebase

# Activate virtual environment
source .venv/bin/activate

# Set CUDA device (change as needed)
export CUDA_VISIBLE_DEVICES=0

# Run training
python ./src/run.py \
    -data_path=./qmsum_data/512 \
    -val_save_file=./outputs/val_out.txt \
    -test_save_file=./outputs/test_results.txt \
    -model=facebook/bart-large \
    -checkpoint=None \
    -log_name=logs/bart_large_ka \
    -gpus='0' \
    -batch_size=1 \
    -learning_rate=5e-6 \
    -num_epochs=10 \
    -warmup=500 \
    -grad_accum=4 \
    -random_seed=42 \
    -do_train \
    -limit_val_batches=1.0 \
    -val_check_interval=1.0 \
    -number_of_segment=6 \
    -max_input_len=512 \
    -max_output_len=256 \
    -min_output_len=64 \
    -n_beams=5 \
    -length_penalty=0.5 \
    -strategy=auto \
    -precision=16

# Uncomment for knowledge-aware mode:
# -knowledge_aware=multihead \

# Clean up cache
find . | grep -E "(__pycache__|\.pyc|\.pyo$)" | xargs rm -rf

echo "Training completed!"
