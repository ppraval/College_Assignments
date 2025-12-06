#!/usr/bin/env python3
"""
Test script to verify KA-QFMS installation
"""
import sys
from pathlib import Path

# Add src directory to path
src_dir = Path(__file__).parent / 'src'
sys.path.insert(0, str(src_dir))

def test_imports():
    """Test that all required packages can be imported"""
    print("Testing imports...")
    
    tests = [
        ("PyTorch", "import torch"),
        ("PyTorch Lightning", "import pytorch_lightning as pl"),
        ("Transformers", "import transformers"),
        ("ROUGE Score", "import rouge_score"),
        ("Jsonlines", "import jsonlines"),
        ("NumPy", "import numpy"),
        ("TensorBoard", "import tensorboard"),
    ]
    
    failed = []
    for name, import_stmt in tests:
        try:
            exec(import_stmt)
            print(f"✓ {name}")
        except ImportError as e:
            print(f"✗ {name}: {e}")
            failed.append(name)
    
    return len(failed) == 0, failed


def test_cuda():
    """Test CUDA availability"""
    print("\nTesting CUDA...")
    import torch
    
    if torch.cuda.is_available():
        print(f"✓ CUDA is available")
        print(f"  - CUDA version: {torch.version.cuda}")
        print(f"  - GPU count: {torch.cuda.device_count()}")
        for i in range(torch.cuda.device_count()):
            print(f"  - GPU {i}: {torch.cuda.get_device_name(i)}")
        return True
    else:
        print("⚠ CUDA is not available (CPU mode only)")
        return False


def test_models():
    """Test that model imports work"""
    print("\nTesting model imports...")
    
    try:
        from models.model import BaseModel
        print("✓ BaseModel")
    except ImportError as e:
        print(f"✗ BaseModel: {e}")
        return False
    
    try:
        from models.fidbart import BartForMultiConditionalGeneration
        print("✓ BartForMultiConditionalGeneration")
    except ImportError as e:
        print(f"✗ BartForMultiConditionalGeneration: {e}")
        return False
    
    try:
        from models.fidpegasus import PegasusForMultiConditionalGeneration
        print("✓ PegasusForMultiConditionalGeneration")
    except ImportError as e:
        print(f"✗ PegasusForMultiConditionalGeneration: {e}")
        return False
    
    try:
        from data_builder import OurDataset
        print("✓ OurDataset")
    except ImportError as e:
        print(f"✗ OurDataset: {e}")
        return False
    
    return True


def main():
    """Run all tests"""
    print("=" * 50)
    print("KA-QFMS Installation Verification")
    print("=" * 50)
    print()
    
    # Test imports
    imports_ok, failed = test_imports()
    
    # Test CUDA
    cuda_ok = test_cuda()
    
    # Test models
    models_ok = test_models()
    
    # Summary
    print("\n" + "=" * 50)
    print("Summary")
    print("=" * 50)
    
    if imports_ok and models_ok:
        print("✓ All tests passed!")
        if not cuda_ok:
            print("⚠ Note: CUDA is not available. Training will be slower.")
        print("\nYou can now run the training script:")
        print("  bash scripts/QMSum.sh")
        return 0
    else:
        print("✗ Some tests failed!")
        if failed:
            print(f"\nFailed imports: {', '.join(failed)}")
        print("\nPlease run the installation script:")
        print("  bash install.sh")
        return 1


if __name__ == "__main__":
    sys.exit(main())
