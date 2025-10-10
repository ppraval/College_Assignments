# Understanding sim-outorder.c: A Pipeline Overview

The doc breaks down the core components and pipeline stages of Simplescalar sim sim-outorder.c . The simulator processes pipeline stages in reverse order in its main loop to ensure consistency between stages.

---

## Pipeline Stages As Executed in sim_main

### 1. `ruu_commit()` - The Commit Stage
* **Core Function**: Retires instructions from the instruction window (RUU/LSQ) to the architectural state in strict program order. This is the final stage where an instruction's results become permanent.
* **Key Operations**:
    * Checks the head of the RUU for completed instructions.
    * For store instructions, writes their data to the cache.
    * Updates the architectural register file with the results.
    * Frees the resources in the RUU and LSQ.
* **Key Data Structures**: RUU, LSQ, Architectural Register File.
* **variables in commit Stage**:
    * 

* **Code Flow**:
    *
---

### 2. `ruu_writeback()` - The Writeback Stage
* **Core Function**: Services instruction completion events from the `event_queue`. It writes back results to the RUU/LSQ and wakes up dependent instructions.
* **Key Operations**:
    * Pulls completed instructions from the `event_queue`.
    * Marks the instruction as "completed" in its RUU/LSQ entry.
    * Broadcasts the result to dependent instructions, marking their corresponding input operands as "ready".
    * If an instruction is newly ready, it's placed on the `ready_queue` for issue.
    * Detects branch mispredictions and initiates pipeline recovery (`ruu_recover`).
* **Key Data Structures**: `event_queue`, RUU, LSQ, `ready_queue`, `create_vector`.
* **variables in Writeback Stage**:
    * `rs` pointer to the RUU_station of the instruction that has just completed execution. it is retrieved from the event queue and contains all the information about the completed instruction, including its output dependency chains that need to be processed. 
    * `link` temporarily holds an entry from the register rename table `create_vector`. It is used to check if the completed instruction is still the original creator of register value before clearing that entry in the rename table. 
    * `olink, olink_next` pointers are used to traverse the output dependency list(`odep_list`) of the completed instruction. `olink` points to the current dependent instruction in the chain and `olink_next` saves the pointer to the next one before the current link is processed and freed. 

* **Code Flow**:
    * writeback while loop continues as long as there are completed events in the event queue for the current cycle. The dequeued next complete instruction is assigned its RUU_Station to rs
        * Instruction validation and status update: ensure the instruction is in a valid state before writeback. 
            * rs->recover_inst reveals a mispredicted branch. this block initiates pipeline recovery. It calls ruu_recover to restores the branch predictor's state and stalls the fetch unit to account for the misprediction penalty.
            * bpred_update to update the branch predictor with the resolved outcome of the branch
                * broadcasting results and waking up dependent registers. This loop reiterates through the output registers of the completed instruction.
                * update the register rename table and clear the entries whose result register is produced by the instruction and clears it. 
            * mark the output dependency chain of all instructions waiting for output results as `TRUE`
                * enqueue the newly ready instructions into the ready queue to be considered for issue in the next cycle.
                * free the RSLinks and output dependency list
---

### 3. `ruu_issue()` - The Issue Stage
* **Core Function**: Selects ready instructions from the `ready_queue` and dispatches them to available functional units for execution. This is where out-of-order execution happens.
* **Key Operations**:
    * Iterates through the `ready_queue`.
    * Checks for an available functional unit using `res_get()`.
    * If a unit is available, reserves it and schedules a completion event in the `event_queue` based on the instruction's latency.
    * If a unit is not available (structural hazard), the instruction is placed back on the `ready_queue`.
* **Key Data Structures**: `ready_queue`, `fu_pool` (Functional Units), `event_queue`.

* **Variables in Issue Stage**:
    * `i, n_issued` - loop counters
    * `load_lat, tlb_lat` - data cache and data TLB latency
    * `node, next_node` - pointers to iterate through ready queue
    * `fu` - pointer to functional unit resource

* **Code Flow**:
    * Issue loop iterates until n_issued doesnt exceed issue width and ready queue contains instructions
        * Checks if the instruction pointed to by the ready queue entry is still valid(not squashed by a branch misprediction). 
            * if store instruction, they are considered issued and completed from the issue stage perspective because their value is written into the LSQ.
            * if other instructions, then check and allocate functional units of the required type
                * if load instruction, the simulator first checks the LSQ for a store-to-load forwarding. it simulates to find total memory access latency
                * once an instruction is issued, a completion event is scheduled in the `event_queue`. The event is scheduled to occur at sim_cycle+ latency 
            * If no functional unit is available, then this models a structural hazard. The instruction is put back onto the ready queue to try again in the next cycle.
            * increment n_issued
        * free the RSLINK(node)

        * loop the instruction not issued back to the ready queue

---

### 4. `ruu_dispatch()` - The Dispatch Stage
* **Core Function**: Decodes instructions, renames registers, and allocates entries in the RUU and LSQ. It establishes the data dependencies between instructions.
* **Key Operations**:
    * Pulls instructions from the Instruction Fetch Queue (`fetch_data`).
    * Allocates a slot in the RUU (and LSQ for memory ops).
    * Uses the `create_vector` (rename table) to link input dependencies (`ruu_link_idep`) and register output dependencies (`ruu_install_odep`).
    * Splits memory operations into two micro-ops (address generation and memory access).
    * Speculatively executes instructions to detect branch mispredictions early.
* **Key Data Structures**: IFQ (`fetch_data`), RUU, LSQ, `create_vector`.

* **Variables in Dispatch Stage**:
    * `i` - iterate through loop to print stats
    * `n_dispatched` - total insts dispatched in current cycle to ensure dispatch width is not exceeded
    * `inst` - actual instruction bits
    * `op` - decoded opcode
    * `out1, out2, in1, in2, in3` - output/input register names
    * `target_PC` - actual next/target PC address
    * `addr` - effective address, if load/store inst
    * `rs` - RUU Station being allocated
    * `lsq` - LSQ Station for ld/st
    * `dir_update_ptr` - direction of the update of the branch pred
    * `stack_recover_idx` - index of RAS top pointer at the time of prediction
    * `pseq` - pipetrace sequence number
    * `is_write` - check if decoded inst is store
    * `br_taken, br_pred_taken` - Flags that hold the actual outcome and predicted outcome of a branch instruction, used to detect mispredictions
    * `fetch_redirected` - check if a branch misfetch was detected and corrected
    * `fault` - Used to capture memory alignment error that occurs during the speculative execution of the instruction

* **Code Flow**:
    * `while` loop drives the dispatch stage. it continues as long as there is available dispatch bandwidth `ruu_decode_width`, free space in the instruction window `RUU_size, LSQ_size`, instructions available in the fetch queue `fetch_num`.
        * if `issue:inorder` is set, the dispatch stage is stalled and breaks the dispatch loop.
        * dequeue the fetch-dispatch queue
        * decode the instruction with `MD_SET_OPCODE` and calculate the default next PC
        * if instruction is trap or system call, stall the dispatch until the instruction window is empty `RUU_num` to ensure the preceding instructions have committed
        * set the Zero register value to 0. 
        * increment the total committed instruction count only if the processor is not in speculative mode.
        * Functional execution - `switch` statement using DEFINST, DEFLINK and `machine.def` to generate a case for every instruction.  
        * if instruction is memory operation, update memory access stats
        * Branch misprediction Detection and Resource Allocation with branch penalty upon fetch.
        * if inst is a non NOP inst, then create an entry in RUU_station which is a combination of reservation station and reorder buffer.
            * if it is a memory operation, then add the entry into load store queue. create dependency links `ruu_link_idep` to link input registers to connect it to the instructions that will produce its source data and `ruu_install_odep` is called for its output register to mark the inst as the producer of that value in the rename table `create_vector`
            * The same process repeats for both RUU and LSQ.
            * once all input operands are available, the ready queue is enqueued
            * create a `RSLINK_INIT` between load store queue entry and the last operation that ruu_dispatch attempted to dispatch. for implementing in-order issue
            * increment instruction execution completion count
        * if inst not in speculative mode, 
            * if control instruction identified, increment the committed branch counter sim_num_branches. if the simulator is configured to update the branch predictor, at the dispatch/decode stage, it calls bpred_update() to update the correct predicted outcome.
            * if a misprediction is found, set the spec_mode = TRUE, indicate that all subsequent instructions are speculative.
        * update the fetch dispatch queue
---

### 5. `ruu_fetch()` - The Fetch Stage
* **Core Function**: Fetches instructions from memory, predicts the direction of branches, and places instructions into the Instruction Fetch Queue.
* **Key Operations**:
    * Determines the next fetch address using the branch predictor (`bpred_lookup`).
    * Simulates I-Cache and ITLB access using `cache_access()`, stalling the pipeline on a miss.
    * Places fetched instructions and prediction metadata into the `fetch_data` queue.
* **Key Data Structures**: Branch Predictor (`pred`), Caches (`cache_il1`, `itlb`), IFQ (`fetch_data`).

* **Notes**: 
    * fetch stage is stalled if ruu_fetch_issue_delay (cache miss latency) is > 0. It implies that fetch stage doesn't run until the instruction is brought to L1i cache from lower memory levels. 

* **Variables in Fetch Stage**:
    * `fetch_regs_PC` - address of the instruction being fetched
    * `fetch_pred_PC` - address of the next instruction to be fetched, predicted by branch predictor
    * `ptrace_seq` - pipetrace sequence number assigned to each instruction
    * `i` - loop counter to count number of instructions fetched in current cycle
    * `lat` - initially latency of I-cache access but updated to actual latency for memory access
    * `tlb_lat` - latency of TLB access. if either of these indicate a miss, fetch stage is stalled
    * `inst` - instruction fetched from memory
    * `stack_recover_idx` - used for return address stack recovery in the branch predictor
    * `branch_cnt` - number of branches fetched in current cycle
    * `done` - flag to indicate if fetch stage for current cycle is done

* **Code Flow**:
    * The `for` loop fetches multiple instructions per cycle. It is constrained by fetch bandwidth,  fetch queue size and branch prediction detection flag
        * `fetch_regs_PC` is set to the addr predicted in the previous iteration. 
        * `if` condition to check for the boundaries of the program's code segment, and also check for address alignment of PC
            * `MD_FETCH_INST` reads instruction from the memory
            * `lat` contains L1i cache hit latency
            * Get L1i cache and tlb latency. Enable `last_inst_missed` and `last_inst_tmissed` flags if there is a miss and set `ruu_fetch_issue_delay` to L2/memory miss latency  
        * `else`, the PC is outside the boundary of code segment. Fill NOP instruction in pipeline

        * `if` branch predictor is configured, then the instruction is pre-decoded.
            * check if it is a control instruction with `F_CTRL` flag and set `fetch_pred_PC` to predicted access of the next instruction to be fetched
            * else set `fetch_pred_PC` to 0

            * if `fetch_pred_PC` has not been set, then set then set it to `fetch_regs_PC` + 4
            * else increment the branch count. if branch count is equal to fetch speed, then set the done flag to `TRUE`
        * `else` no predictor. `fetch_pred_PC` `fetch_regs_PC` + 4

        * `fetch_data` fetch queue is enqueued 

---

## Key Data Structures for Out-of-Order Execution

* **`RUU_station`**: An entry in the RUU or LSQ. It holds the instruction, its dependencies, its status (queued, issued, completed), and pointers to dependent instructions.
* **`create_vector`**: The register rename table. An array that maps an architectural register to the `RUU_station` that will next write to it, enabling the breaking of false dependencies.
* **`RS_link`**: A generic linked-list node used to build the dependency chains, the `ready_queue`, and the `event_queue`.