# Makefile for CUDA LU Factorization Solver

# CUDA compiler
NVCC = nvcc

# Compiler flags
NVCC_FLAGS = -O3 -arch=sm_60 -Xcompiler -Wall

# Target executable
TARGET = lu_solver

# Source files
SRC = lu_solver.cu

# Input files directory
INPUT_DIR = linear_equation_solver_input_files

# Default target
all: $(TARGET)

# Compile the CUDA program
$(TARGET): $(SRC)
	$(NVCC) $(NVCC_FLAGS) -o $(TARGET) $(SRC)

# Run with all input files
run_all: $(TARGET)
	@echo "Running LU Solver with all input files..."
	@echo "=========================================="
	./$(TARGET) $(INPUT_DIR)/indata50.txt
	@echo ""
	./$(TARGET) $(INPUT_DIR)/indata80.txt
	@echo ""
	./$(TARGET) $(INPUT_DIR)/indata100.txt
	@echo ""
	./$(TARGET) $(INPUT_DIR)/indata200.txt
	@echo ""
	./$(TARGET) $(INPUT_DIR)/indata500.txt

# Run with specific input file (e.g., make run N=50)
run: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata$(N).txt

# Run with indata50.txt
run50: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata50.txt

# Run with indata80.txt
run80: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata80.txt

# Run with indata100.txt
run100: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata100.txt

# Run with indata200.txt
run200: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata200.txt

# Run with indata500.txt
run500: $(TARGET)
	./$(TARGET) $(INPUT_DIR)/indata500.txt

# Clean generated files
clean:
	rm -f $(TARGET) output_*.txt timing_*.txt

# Clean only output files
clean_output:
	rm -f output_*.txt timing_*.txt

# Help
help:
	@echo "Available targets:"
	@echo "  make          - Compile the program"
	@echo "  make run_all  - Run with all input files"
	@echo "  make run50    - Run with indata50.txt"
	@echo "  make run80    - Run with indata80.txt"
	@echo "  make run100   - Run with indata100.txt"
	@echo "  make run200   - Run with indata200.txt"
	@echo "  make run500   - Run with indata500.txt"
	@echo "  make run N=XX - Run with indataXX.txt"
	@echo "  make clean    - Remove executable and output files"
	@echo "  make clean_output - Remove only output files"

.PHONY: all run_all run run50 run80 run100 run200 run500 clean clean_output help
