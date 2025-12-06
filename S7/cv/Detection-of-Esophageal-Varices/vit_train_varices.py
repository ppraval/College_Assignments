from datasets import load_dataset, DatasetDict
import random
from PIL import ImageDraw, Image
from transformers import ViTFeatureExtractor, ViTForImageClassification, Trainer, TrainingArguments
import torch
import evaluate
import numpy as np

# Load your dataset (single folder with class subfolders)
dataset = load_dataset("imagefolder", data_dir="ev_data")
# Verify the class names
print("Original class names:", dataset['train'].features['label'].names)

# Manually split into train and validation (80-20 split)
split = dataset['train'].train_test_split(test_size=0.2, seed=42)
ds = DatasetDict({
    'train': split['train'],
    'validation': split['test']
})

# Verify the splits
print(f"Training samples: {len(ds['train'])}")
print(f"Validation samples: {len(ds['validation'])}")

# Initialize feature extractor
model_name_or_path = 'google/vit-base-patch16-224-in21k'
feature_extractor = ViTFeatureExtractor.from_pretrained(model_name_or_path)

# Transformation functions
def transform(example_batch):
    inputs = feature_extractor([x for x in example_batch['image']], return_tensors='pt')
    inputs['labels'] = example_batch['label']  # Note: Trainer expects 'labels' (plural)
    return inputs

prepared_ds = ds.with_transform(transform)

def collate_fn(batch):
    return {
        'pixel_values': torch.stack([x['pixel_values'] for x in batch]),
        'labels': torch.tensor([x['labels'] for x in batch])
    }

# Metrics
metric = evaluate.load("accuracy")
def compute_metrics(p):
    return metric.compute(
        predictions=np.argmax(p.predictions, axis=1),
        references=p.label_ids
    )

# Model initialization
labels = ds['train'].features['label'].names
model = ViTForImageClassification.from_pretrained(
    model_name_or_path,
    num_labels=len(labels),
    id2label={str(i): c for i, c in enumerate(labels)},
    label2id={c: str(i) for i, c in enumerate(labels)}
)

# Training arguments
training_args = TrainingArguments(
    output_dir="./vit-base-esophagus-demo",
    per_device_train_batch_size=16,  # Reduced if you have memory issues
    eval_strategy="steps",
    num_train_epochs=4,
    fp16=True,
    save_steps=100,
    eval_steps=100,
    logging_steps=10,
    learning_rate=2e-4,
    save_total_limit=2,
    remove_unused_columns=False,
    push_to_hub=False,
    report_to='tensorboard',
    load_best_model_at_end=True,
)

# Trainer
trainer = Trainer(
    model=model,
    args=training_args,
    data_collator=collate_fn,
    compute_metrics=compute_metrics,
    train_dataset=prepared_ds["train"],
    eval_dataset=prepared_ds["validation"],
    tokenizer=feature_extractor,
)

# Train and evaluate
train_results = trainer.train()
trainer.save_model()
trainer.log_metrics("train", train_results.metrics)
trainer.save_metrics("train", train_results.metrics)
trainer.save_state()

metrics = trainer.evaluate(prepared_ds['validation'])
trainer.log_metrics("eval", metrics)
trainer.save_metrics("eval", metrics)