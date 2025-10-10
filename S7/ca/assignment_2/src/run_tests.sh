#!/bin/bash

# Script to run all tests and generate summary
# Usage: ./run_tests.sh

echo "=========================================="
echo "CUDA LU Factorization Solver - Test Suite"
echo "=========================================="
echo ""

# Check if executable exists
if [ ! -f "./lu_solver" ]; then
    echo "Executable not found. Compiling..."
    make
    echo ""
fi

# Array of test sizes
SIZES=(3 50 80 100 200 500)

echo "Running tests for all input sizes..."
echo ""

# Run each test
for N in "${SIZES[@]}"; do
    echo "----------------------------------------"
    echo "Testing with N = $N"
    echo "----------------------------------------"
    ./lu_solver linear_equation_solver_input_files/indata${N}.txt
    echo ""
done

echo "=========================================="
echo "All tests completed!"
echo "=========================================="
echo ""

# Generate summary
echo "SUMMARY OF RESULTS"
echo "=================="
echo ""
printf "%-6s %-12s %-12s %-12s %-15s\n" "N" "I/O (ms)" "L (ms)" "U (ms)" "Total (ms)"
echo "--------------------------------------------------------------"

for N in "${SIZES[@]}"; do
    if [ -f "timing_${N}.txt" ]; then
        IO_TIME=$(grep "read A and B" timing_${N}.txt | awk '{print $NF}')
        L_TIME=$(grep "compute L" timing_${N}.txt | awk '{print $NF}')
        U_TIME=$(grep "compute U" timing_${N}.txt | awk '{print $NF}')
        TOTAL_TIME=$(grep "solving linear" timing_${N}.txt | awk '{print $NF}')
        printf "%-6s %-12s %-12s %-12s %-15s\n" "$N" "$IO_TIME" "$L_TIME" "$U_TIME" "$TOTAL_TIME"
    fi
done

echo ""
echo "Output files generated:"
for N in "${SIZES[@]}"; do
    if [ -f "output_${N}.txt" ]; then
        SIZE=$(ls -lh output_${N}.txt | awk '{print $5}')
        echo "  - output_${N}.txt (${SIZE})"
    fi
done

echo ""
echo "Timing files generated:"
for N in "${SIZES[@]}"; do
    if [ -f "timing_${N}.txt" ]; then
        echo "  - timing_${N}.txt"
    fi
done

echo ""
echo "For detailed analysis, see:"
echo "  - REPORT.md (comprehensive technical report)"
echo "  - CONSOLIDATED_TIMING_REPORT.txt (performance summary)"
echo "  - README.md (project documentation)"
