from transformers import ViTForImageClassification, ViTFeatureExtractor
from PIL import Image
import os
import glob

# 1. Configuration - Set your paths here
model_dir = os.path.abspath("vit-base-esophagus-demo/checkpoint-560")
images_folder = "testing_varices"  # Folder containing your images

# 2. Load model and processor (your original working version)
model = ViTForImageClassification.from_pretrained(
    model_dir,
    local_files_only=True
)
feature_extractor = ViTFeatureExtractor.from_pretrained(
    model_dir,
    local_files_only=True
)

# 3. Your original prediction function (unchanged)
def predict(image_path):
    img = Image.open(image_path).convert("RGB")  # Handles BMP conversion automatically
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

# 4. Batch processing for all images in folder (now includes BMP)
if __name__ == "__main__":
    print(f"Model loaded from: {model_dir}")
    print(f"Processing ALL images from: {os.path.abspath(images_folder)}\n")

    # Get all image files (supports .jpg, .png, .jpeg, .bmp)
    image_types = ("*.jpg", "*.jpeg", "*.png", "*.bmp")
    image_paths = []
    for ext in image_types:
        image_paths.extend(glob.glob(os.path.join(images_folder, ext)))

    if not image_paths:
        print(f"No images found in {images_folder}")
        print("Supported formats: JPG, JPEG, PNG, BMP")
    else:
        for img_path in image_paths:
            try:
                result = predict(img_path)
                filename = os.path.basename(img_path)
                print(f"Image: {filename}")
                print(f"• Prediction: {result['prediction']}")
                print(f"• Confidence: {result['confidence']}")
                print(f"• Varices: {result['probabilities']['varices']}")
                print(f"• Healthy: {result['probabilities']['healthy']}\n")
            except Exception as e:
                print(f"Error processing {os.path.basename(img_path)}: {str(e)}\n")

        print(f"Completed! Processed {len(image_paths)} images.")