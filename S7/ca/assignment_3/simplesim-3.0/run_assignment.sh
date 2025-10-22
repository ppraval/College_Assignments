#!/bin/bash

# Cache Replacement Policy Analysis Script
# This script runs LRU vs MRU comparison experiments
# Date: September 27, 2025

echo "=========================================="
echo "Cache Replacement Policy Analysis Script"
echo "LRU vs MRU Performance Comparison"
echo "=========================================="

# Create results directory
mkdir -p assignment_results
cd assignment_results

echo "Starting benchmark execution..."

# Test 1: Alpha test_fmath with LRU
echo "Running Alpha test_fmath with LRU configuration..."
../sim-outorder -config ../baseline_lru.cfg ../tests-alpha/bin/test-fmath > alpha_lru_results.txt 2>&1
echo "Alpha LRU test completed."

# Test 2: Alpha test_fmath with MRU  
echo "Running Alpha test_fmath with MRU configuration..."
../sim-outorder -config ../baseline_mru.cfg ../tests-alpha/bin/test-fmath > alpha_mru_results.txt 2>&1
echo "Alpha MRU test completed."

# Test 3: PISA test_lswlr with LRU
echo "Running PISA test_lswlr with LRU configuration..."
if [ -f ../tests-pisa/bin.little/test-lswlr ]; then
    ../sim-outorder -config ../baseline_lru.cfg ../tests-pisa/bin.little/test-lswlr > pisa_lru_results.txt 2>&1
    echo "PISA LRU test completed."
else
    echo "PISA test-lswlr binary not found, skipping PISA tests..."
fi

# Test 4: PISA test_lswlr with MRU
echo "Running PISA test_lswlr with MRU configuration..."
if [ -f ../tests-pisa/bin.little/test-lswlr ]; then
    ../sim-outorder -config ../baseline_mru.cfg ../tests-pisa/bin.little/test-lswlr > pisa_mru_results.txt 2>&1
    echo "PISA MRU test completed."
else
    echo "PISA test-lswlr binary not found, skipping PISA tests..."
fi

echo "All benchmark executions completed!"

# Extract key metrics
echo ""
echo "=========================================="
echo "EXTRACTING KEY PERFORMANCE METRICS"
echo "=========================================="

# Create summary file
echo "Cache Replacement Policy Analysis Results" > analysis_summary.txt
echo "Generated on: $(date)" >> analysis_summary.txt  
echo "==========================================" >> analysis_summary.txt
echo "" >> analysis_summary.txt

# Function to extract metrics from a result file
extract_metrics() {
    local file=$1
    local benchmark=$2
    local policy=$3
    
    echo "Extracting metrics from $file..."
    
    if [ -f "$file" ]; then
        echo "Benchmark: $benchmark | Policy: $policy" >> analysis_summary.txt
        echo "----------------------------------------" >> analysis_summary.txt
        
        # Extract simulation cycles
        grep "sim_cycle" "$file" >> analysis_summary.txt
        
        # Extract IPC 
        grep "sim_IPC" "$file" >> analysis_summary.txt
        
        # Extract instruction count
        grep "sim_num_insn" "$file" >> analysis_summary.txt
        
        # Extract cache statistics
        grep "dl1\.accesses\|dl1\.hits\|dl1\.misses\|dl1\.miss_rate" "$file" >> analysis_summary.txt
        grep "il1\.accesses\|il1\.hits\|il1\.misses\|il1\.miss_rate" "$file" >> analysis_summary.txt  
        grep "ul2\.accesses\|ul2\.hits\|ul2\.misses\|ul2\.miss_rate" "$file" >> analysis_summary.txt
        
        echo "" >> analysis_summary.txt
    else
        echo "File $file not found!" >> analysis_summary.txt
        echo "" >> analysis_summary.txt
    fi
}

# Extract metrics for all tests
extract_metrics "alpha_lru_results.txt" "Alpha_test_fmath" "LRU"
extract_metrics "alpha_mru_results.txt" "Alpha_test_fmath" "MRU"
extract_metrics "pisa_lru_results.txt" "PISA_test_lswlr" "LRU"  
extract_metrics "pisa_mru_results.txt" "PISA_test_lswlr" "MRU"

echo "Metric extraction completed!"

# Create comparison table
echo "" >> analysis_summary.txt
echo "PERFORMANCE COMPARISON TABLE" >> analysis_summary.txt
echo "============================" >> analysis_summary.txt
echo "Format: Benchmark | Policy | Cycles | IPC | L1D_Miss_Rate | L1I_Miss_Rate | L2_Miss_Rate" >> analysis_summary.txt
echo "" >> analysis_summary.txt

# Function to create comparison row
create_comparison_row() {
    local file=$1
    local benchmark=$2
    local policy=$3
    
    if [ -f "$file" ]; then
        local cycles=$(grep "sim_cycle" "$file" | awk '{print $2}')
        local ipc=$(grep "sim_IPC" "$file" | awk '{print $2}')
        local dl1_miss=$(grep "dl1.miss_rate" "$file" | awk '{print $2}')
        local il1_miss=$(grep "il1.miss_rate" "$file" | awk '{print $2}')
        local ul2_miss=$(grep "ul2.miss_rate" "$file" | awk '{print $2}')
        
        echo "$benchmark | $policy | $cycles | $ipc | $dl1_miss | $il1_miss | $ul2_miss" >> analysis_summary.txt
    else
        echo "$benchmark | $policy | N/A | N/A | N/A | N/A | N/A" >> analysis_summary.txt
    fi
}

create_comparison_row "alpha_lru_results.txt" "Alpha" "LRU"
create_comparison_row "alpha_mru_results.txt" "Alpha" "MRU"
create_comparison_row "pisa_lru_results.txt" "PISA" "LRU"
create_comparison_row "pisa_mru_results.txt" "PISA" "MRU"

echo "" >> analysis_summary.txt

# Extract specific metrics for easy plotting
echo "QUICK METRICS EXTRACTION" >> analysis_summary.txt
echo "========================" >> analysis_summary.txt
echo "" >> analysis_summary.txt

echo "L1 Data Cache Miss Rates:" >> analysis_summary.txt
echo "Alpha LRU:" >> analysis_summary.txt
grep "dl1.miss_rate" alpha_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "Alpha MRU:" >> analysis_summary.txt  
grep "dl1.miss_rate" alpha_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA LRU:" >> analysis_summary.txt
grep "dl1.miss_rate" pisa_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA MRU:" >> analysis_summary.txt
grep "dl1.miss_rate" pisa_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "" >> analysis_summary.txt

echo "Simulation Cycles:" >> analysis_summary.txt
echo "Alpha LRU:" >> analysis_summary.txt
grep "sim_cycle" alpha_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "Alpha MRU:" >> analysis_summary.txt
grep "sim_cycle" alpha_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA LRU:" >> analysis_summary.txt
grep "sim_cycle" pisa_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA MRU:" >> analysis_summary.txt
grep "sim_cycle" pisa_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "" >> analysis_summary.txt

echo "IPC Values:" >> analysis_summary.txt
echo "Alpha LRU:" >> analysis_summary.txt
grep "sim_IPC" alpha_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "Alpha MRU:" >> analysis_summary.txt
grep "sim_IPC" alpha_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA LRU:" >> analysis_summary.txt
grep "sim_IPC" pisa_lru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt
echo "PISA MRU:" >> analysis_summary.txt
grep "sim_IPC" pisa_mru_results.txt 2>/dev/null >> analysis_summary.txt || echo "N/A" >> analysis_summary.txt

echo ""
echo "=========================================="
echo "ANALYSIS COMPLETE!"
echo "=========================================="
echo "Results saved in: assignment_results/"
echo "Summary file: assignment_results/analysis_summary.txt"
echo ""
echo "Individual result files:"
echo "- alpha_lru_results.txt"  
echo "- alpha_mru_results.txt"
echo "- pisa_lru_results.txt"
echo "- pisa_mru_results.txt"
echo ""
echo "Next steps:"
echo "1. Review analysis_summary.txt for key metrics"
echo "2. Calculate normalized values (MRU/LRU ratios)"
echo "3. Create plots using the extracted data"
echo "4. Write your analysis report"

cd ..  # Return to original directory