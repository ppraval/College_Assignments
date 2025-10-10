CACHE REPLACEMENT POLICY ANALYSIS ASSIGNMENT
=============================================
Student Assignment Submission Package
Date: September 27, 2025
Topic: LRU vs MRU Cache Replacement Policy Comparison

ASSIGNMENT OVERVIEW
===================
This package contains a complete analysis of cache replacement policies (LRU vs MRU) 
using SimpleScalar simulator with the baseline system configuration specified in Table 1.

MAIN DELIVERABLES
=================

1. **Complete_Analysis_Report.txt** - PRIMARY ASSIGNMENT REPORT
   - Executive summary with key findings
   - Detailed experimental results for both benchmarks
   - Performance comparison tables and normalized data
   - Theoretical analysis and justification
   - Final recommendation with scientific evidence
   - Ready-to-use plotting data

2. **Assignment_Analysis_Guide.txt** - METHODOLOGY DOCUMENTATION
   - Step-by-step execution guide
   - Configuration parameter calculations
   - Expected results and analysis framework
   - Evaluation guidelines implementation

SUPPORTING FILES
================

3. **Command_Reference.txt** - REPRODUCTION GUIDE
   - All commands needed to reproduce results
   - Quick reference for key metrics
   - Expected output summary
   - File organization

4. **MRU_Implementation_Guide.txt** - TECHNICAL IMPLEMENTATION
   - Complete MRU replacement policy implementation
   - Code changes made to SimpleScalar
   - Build and test instructions
   - Technical verification steps

CONFIGURATION FILES
===================

5. **baseline_lru.cfg** - LRU Configuration
   - Matches Table 1 baseline system specification
   - 32KB L1 caches (I/D), 256KB L2, 64-byte blocks
   - LRU replacement policy throughout

6. **baseline_mru.cfg** - MRU Configuration  
   - Identical to LRU config except replacement policy
   - Uses MRU (Most Recently Used) replacement
   - Enables direct performance comparison

AUTOMATION SCRIPT
=================

7. **run_assignment.sh** - AUTOMATED EXECUTION
   - Runs all benchmarks with both policies
   - Extracts key performance metrics automatically
   - Generates comparison tables
   - Creates analysis summary

EXPERIMENTAL RESULTS
====================

8. **assignment_results/** - RAW SIMULATION DATA
   - alpha_lru_results.txt (test_fmath with LRU)
   - alpha_mru_results.txt (test_fmath with MRU)
   - alpha_lswlr_lru_results.txt (test_lswlr with LRU)
   - alpha_lswlr_mru_results.txt (test_lswlr with MRU)
   - analysis_summary.txt (automated metric extraction)

KEY EXPERIMENTAL FINDINGS
==========================

BENCHMARKS TESTED:
- Alpha test_fmath (floating-point math operations)
- Alpha test_lswlr (load-store word left/right operations)

PERFORMANCE RESULTS:
                    | test_fmath      | test_lswlr      | Average
--------------------|-----------------|-----------------|----------
Execution Time      | LRU 44% better  | LRU 9% better   | 26.6% better
IPC                 | LRU 30% better  | LRU 8% better   | 19.5% better  
L1D Miss Rate       | LRU 52% better  | LRU 16% better  | 33.9% better
L1I Miss Rate       | LRU 137% better | LRU 20% better  | 78.6% better

CONCLUSION: LRU consistently and significantly outperforms MRU across all metrics.

PLOTTING DATA (Assignment Requirement)
======================================

For Required Plots:

1. **Absolute L1 Miss Rate Comparison:**
Benchmark    | LRU L1D | MRU L1D | LRU L1I | MRU L1I
-------------|---------|---------|---------|--------
test_fmath   | 3.24%   | 4.92%   | 1.55%   | 3.67%
test_lswlr   | 7.93%   | 9.19%   | 4.16%   | 5.01%

2. **Normalized Performance (MRU/LRU, where LRU = 1.0):**
Metric          | test_fmath | test_lswlr | Average
----------------|------------|------------|--------
Cycles          | 1.44       | 1.09       | 1.27
IPC             | 0.69       | 0.92       | 0.81
L1D Miss Rate   | 1.52       | 1.16       | 1.34
L1I Miss Rate   | 2.37       | 1.20       | 1.79

Values > 1.0 indicate MRU performs worse than LRU baseline.

3. **Cycle Time Comparison (Absolute Values):**
Benchmark    | LRU Cycles | MRU Cycles | MRU Penalty
-------------|------------|------------|------------
test_fmath   | 96,116     | 138,368    | +44.0%
test_lswlr   | 66,196     | 72,283     | +9.2%

ASSIGNMENT REQUIREMENTS FULFILLMENT
===================================

✅ Task 1: Analyzed and compared LRU and MRU replacement policies
✅ Task 2: Identified LRU as best-performing policy with quantitative evidence  
✅ Task 3: Provided detailed plots data and comprehensive explanation
✅ Task 4: Included miss rate vs cycle time and L1 miss rate analysis

✅ Evaluation Guidelines:
   - Used LRU as baseline for normalization
   - Provided absolute L1 miss rate values
   - Created normalized parameters (MRU/LRU ratios)

TECHNICAL IMPLEMENTATION
========================

MRU Replacement Policy Implementation:
- Modified cache.h to add MRU enum
- Updated cache.c with MRU logic and parsing
- Modified sim-outorder.c and sim-cache.c documentation
- Verified MRU behaves differently from LRU (higher miss rates)

System Configuration Verification:
- Matches Table 1 specification exactly
- Proper cache sizing calculations included
- All performance parameters configured correctly

REPRODUCTION INSTRUCTIONS
=========================

To reproduce these results:

1. Navigate to SimpleScalar directory
2. Use provided configuration files (baseline_lru.cfg, baseline_mru.cfg)
3. Run: ./run_assignment.sh
4. Or execute individual commands from Command_Reference.txt
5. Results will be generated in assignment_results/ directory

RECOMMENDATION
==============

**BEST PERFORMING CACHE REPLACEMENT POLICY: LRU**

Scientific Evidence:
- 26.6% better average execution time
- 19.5% better average IPC
- 33.9% lower L1 data cache miss rates
- 78.6% lower L1 instruction cache miss rates
- Consistent benefits across different workload types

Theoretical Justification:
LRU exploits temporal locality principle effectively, keeping recently accessed 
data in cache longer, while MRU violates this principle by evicting recently 
used blocks that are likely to be accessed again soon.

PACKAGE CONTENTS SUMMARY
========================
- 1 Primary Report (Complete_Analysis_Report.txt)
- 3 Supporting Documentation Files  
- 2 Configuration Files (LRU/MRU)
- 1 Automation Script
- 5 Raw Result Files in assignment_results/
- 1 README (this file)

Total: 13 files providing complete assignment solution

END OF README
=============