# Detection of Esophageal Varices

A deep learning project using Vision Transformer (ViT) for automated detection of esophageal varices from medical images.

## Overview

This project implements a binary classification system to detect esophageal varices using a fine-tuned Vision Transformer model. The system can analyze medical images and classify them as either showing esophageal varices or healthy esophagus tissue.

## Features

- **Vision Transformer Architecture**: Uses Google's ViT-base-patch16-224-in21k pre-trained model
- **Binary Classification**: Distinguishes between esophagus varices and healthy esophagus
- **Batch Inference**: Process multiple images at once
- **Data Organization**: Automated filtering and organization based on medical reports
- **High Accuracy**: Fine-tuned model with evaluation metrics tracking

## Project Structure

```
Detection-of-Esophageal-Varices/
├── vit_train_varices.py          # Model training script
├── vit_inference_varices.py      # Single image inference
├── vit_folder_inference_varices.py # Batch image inference
├── vit_test.py                   # Testing/experimental code
├── varices_filter.py             # PDF-based data filtering utility
├── requirements.txt              # Python dependencies
├── commands.txt                  # Setup and migration notes
└── README.md                     # This file
```

## Requirements

- Python 3.8+
- PyTorch with CUDA 12.6 support (recommended)
- Transformers library
- See `requirements.txt` for complete list

## Installation

### 1. Install PyTorch with CUDA support

```bash
python -m pip install --upgrade pip
pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu126
```

### 2. Install dependencies

```bash
pip install datasets transformers
pip install 'accelerate>=0.26.0'
pip install transformers[torch]
pip install scikit-learn
pip install tensorboardX
pip install evaluate
```

Or simply:

```bash
pip install -r requirements.txt
```

## Usage

### Training the Model

Prepare your dataset in the following structure:
```
ev_data/
├── esophagus_varices/
│   ├── image1.jpg
│   ├── image2.jpg
│   └── ...
└── healthy_esophagus/
    ├── image1.jpg
    ├── image2.jpg
    └── ...
```

Run the training script:

```bash
python vit_train_varices.py
```

**Training Parameters:**
- Batch size: 16
- Epochs: 4
- Learning rate: 2e-4
- Train/validation split: 80/20
- FP16 mixed precision training enabled
- Checkpoint saving every 100 steps

### Single Image Inference

```bash
python vit_inference_varices.py
```

Modify the `image_path` variable in the script to point to your test image.

**Output:**
- Predicted condition (varices/healthy)
- Confidence score
- Probability distribution for both classes

### Batch Inference

Process an entire folder of images:

```bash
python vit_folder_inference_varices.py
```

Configure the script by modifying:
- `model_dir`: Path to trained model checkpoint
- `images_folder`: Directory containing images to process

**Supported formats:** JPG, JPEG, PNG, BMP

### Data Filtering

Organize medical data based on PDF reports:

```bash
python varices_filter.py
```

This script:
- Scans folders for PDF reports
- Searches for "esophageal varices" text
- Automatically organizes images into varices/no-varices folders

## Model Details

**Base Model:** `google/vit-base-patch16-224-in21k`

**Architecture:**
- Vision Transformer with patch size 16x16
- Input image size: 224x224
- Pre-trained on ImageNet-21k
- Fine-tuned for binary classification

**Training Configuration:**
- Optimizer: AdamW (default)
- Evaluation strategy: Steps-based
- Best model selection based on validation accuracy
- TensorBoard logging enabled

## Output Structure

After training, the model saves to:
```
vit-base-esophagus-demo/
├── checkpoint-XXX/
│   ├── config.json
│   ├── model.safetensors
│   ├── preprocessor_config.json
│   └── trainer_state.json
└── ...
```

## Performance Metrics

The model tracks:
- Training accuracy
- Validation accuracy
- Training loss
- Validation loss

View metrics using TensorBoard:
```bash
tensorboard --logdir=./vit-base-esophagus-demo
```

## Notes

- The project uses the updated `evaluate` library instead of deprecated `load_metric`
- Training argument `evaluation_strategy` has been changed to `eval_strategy` for compatibility
- Model supports both CPU and GPU inference (CUDA recommended)

## Data Requirements

- Images should be clear endoscopic images
- Recommended image quality: High resolution
- Proper labeling required for training data
- Balanced dataset recommended for best performance

## Troubleshooting

**Memory Issues:**
- Reduce `per_device_train_batch_size` in training arguments
- Disable FP16 if having GPU compatibility issues

**Path Issues:**
- Use absolute paths when specifying model and data directories
- Ensure proper file permissions for reading/writing

**Missing Dependencies:**
- Verify CUDA version matches PyTorch installation
- Install missing packages from `requirements.txt`

## Future Improvements

- [ ] Multi-class classification for severity levels
- [ ] Real-time inference API
- [ ] Model quantization for faster inference
- [ ] Web-based deployment
- [ ] Data augmentation techniques
- [ ] Cross-validation implementation

## License

This project is for educational/research purposes.

## Acknowledgments

- Google Research for the Vision Transformer architecture
- Hugging Face for the Transformers library
- Medical dataset contributors

---

**Note:** This is a medical image classification tool intended for research purposes. Always consult qualified medical professionals for diagnosis and treatment decisions.
