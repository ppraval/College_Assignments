from transformers import ViTForImageClassification, ViTFeatureExtractor
from PIL import Image
import os

# 1. FIXED MODEL PATH
model_dir = os.path.abspath("vit-base-esophagus-demo/checkpoint-560")

# 2. Load model and processor
model = ViTForImageClassification.from_pretrained(
    model_dir,
    local_files_only=True
)
feature_extractor = ViTFeatureExtractor.from_pretrained(
    model_dir,
    local_files_only=True
)

# 3. Prediction function (unchanged)
def predict(image_path):
    img = Image.open(image_path).convert("RGB")
    inputs = feature_extractor(images=img, return_tensors="pt")
    outputs = model(**inputs)
    probs = outputs.logits.softmax(dim=-1)

    return {
        "prediction": ["esophagus_varices", "healthy_esophagus"][probs.argmax().item()],
        "confidence": f"{probs.max().item():.1%}",
        "probabilities": {
            "varices": f"{probs[0][0].item():.1%}",
            "healthy": f"{probs[0][1].item():.1%}"
        }
    }

# 4. Run inference (with path verification)
if __name__ == "__main__":
    image_path = "ev_data/esophagus_varices/cropped_00251f56-CP250109_133313_030_rot0.jpg"

    # Verify paths
    print(f"Model directory: {model_dir}")
    print(f"Image path: {os.path.abspath(image_path)}")

    result = predict(image_path)
    print("\nDiagnosis Results:")
    print(f"• Condition: {result['prediction']}")
    print(f"• Confidence: {result['confidence']}")
    print(f"• Varices probability: {result['probabilities']['varices']}")
    print(f"• Healthy probability: {result['probabilities']['healthy']}")