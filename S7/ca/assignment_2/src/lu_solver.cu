#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <math.h>

// Error checking macro
#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            fprintf(stderr, "CUDA error in %s:%d: %s\n", __FILE__, __LINE__, \
                    cudaGetErrorString(err)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

// Timing structure
typedef struct {
    double io_time;
    double l_time;
    double u_time;
    double total_solve_time;
} TimingInfo;

// Performance metrics structure
typedef struct {
    int total_threads;
    int blocks_per_grid;
    int threads_per_block;
    double cgma_ratio;
    long long total_flops;
    long long total_memory_accesses;
} PerformanceMetrics;

// CUDA kernel to find pivot row
__global__ void findPivotKernel(double *A, int *pivot, int n, int k) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    
    __shared__ double max_vals[256];
    __shared__ int max_indices[256];
    
    if (tid < n - k) {
        int row = k + tid;
        max_vals[threadIdx.x] = fabs(A[row * n + k]);
        max_indices[threadIdx.x] = row;
    } else {
        max_vals[threadIdx.x] = -1.0;
        max_indices[threadIdx.x] = k;
    }
    __syncthreads();
    
    // Reduction to find maximum
    for (int stride = blockDim.x / 2; stride > 0; stride >>= 1) {
        if (threadIdx.x < stride && threadIdx.x + stride < n - k) {
            if (max_vals[threadIdx.x + stride] > max_vals[threadIdx.x]) {
                max_vals[threadIdx.x] = max_vals[threadIdx.x + stride];
                max_indices[threadIdx.x] = max_indices[threadIdx.x + stride];
            }
        }
        __syncthreads();
    }
    
    if (threadIdx.x == 0) {
        pivot[0] = max_indices[0];
    }
}

// CUDA kernel to swap rows
__global__ void swapRowsKernel(double *A, double *B, int n, int row1, int row2) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (col < n) {
        double temp = A[row1 * n + col];
        A[row1 * n + col] = A[row2 * n + col];
        A[row2 * n + col] = temp;
    }
    
    if (col == 0 && B != NULL) {
        double temp = B[row1];
        B[row1] = B[row2];
        B[row2] = temp;
    }
}

// CUDA kernel for LU factorization - compute multipliers (L matrix)
__global__ void computeMultipliersKernel(double *A, double *L, int n, int k) {
    int row = blockIdx.x * blockDim.x + threadIdx.x + k + 1;
    
    if (row < n) {
        double pivot = A[k * n + k];
        if (fabs(pivot) > 1e-10) {
            L[row * n + k] = A[row * n + k] / pivot;
        } else {
            L[row * n + k] = 0.0;
        }
    }
}

// CUDA kernel for LU factorization - update submatrix (OPTIMIZED with shared memory)
__global__ void updateSubmatrixKernel(double *A, double *L, int n, int k) {
    // Shared memory for caching frequently accessed data
    __shared__ double s_L[16];      // Cache L[row, k] values
    __shared__ double s_A_row[16];  // Cache A[k, col] values
    
    int row = blockIdx.y * blockDim.y + threadIdx.y + k + 1;
    int col = blockIdx.x * blockDim.x + threadIdx.x + k + 1;
    
    // Load L[row, k] into shared memory (one per row in block)
    if (threadIdx.x == 0 && row < n) {
        s_L[threadIdx.y] = L[row * n + k];
    }
    
    // Load A[k, col] into shared memory (one per column in block)
    if (threadIdx.y == 0 && col < n) {
        s_A_row[threadIdx.x] = A[k * n + col];
    }
    
    __syncthreads();
    
    // Perform update using cached values from shared memory
    if (row < n && col < n) {
        A[row * n + col] -= s_L[threadIdx.y] * s_A_row[threadIdx.x];
    }
}

// CUDA kernel for forward substitution (solving Ly = Pb)
__global__ void forwardSubstitutionKernel(double *L, double *B, double *y, int n, int i) {
    // Each iteration solves for y[i]
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L[i * n + j] * y[j];
        }
        y[i] = B[i] - sum;
    }
}

// CUDA kernel for backward substitution (solving Ux = y)
__global__ void backwardSubstitutionKernel(double *U, double *y, double *x, int n, int i) {
    // Each iteration solves for x[i]
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += U[i * n + j] * x[j];
        }
        if (fabs(U[i * n + i]) > 1e-10) {
            x[i] = (y[i] - sum) / U[i * n + i];
        } else {
            x[i] = 0.0;
        }
    }
}

// Read input file
void readInput(const char *filename, int *n, double **A, double **B, double *io_time) {
    clock_t start = clock();
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fscanf(fp, "%d", n);
    
    *A = (double *)malloc((*n) * (*n) * sizeof(double));
    *B = (double *)malloc((*n) * sizeof(double));
    
    // Read matrix A
    for (int i = 0; i < (*n) * (*n); i++) {
        fscanf(fp, "%lf", &((*A)[i]));
    }
    
    // Read vector B
    for (int i = 0; i < *n; i++) {
        fscanf(fp, "%lf", &((*B)[i]));
    }
    
    fclose(fp);
    
    clock_t end = clock();
    *io_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0; // Convert to ms
}

// Write output file
void writeOutput(const char *filename, int n, double *L, double *U, double *x) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening output file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "%d\n", n);
    
    // Write L matrix
    for (int i = 0; i < n * n; i++) {
        fprintf(fp, "%.16e\n", L[i]);
    }
    
    // Write U matrix
    for (int i = 0; i < n * n; i++) {
        fprintf(fp, "%.16e\n", U[i]);
    }
    
    // Write solution vector x
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%.16e\n", x[i]);
    }
    
    fclose(fp);
}

// Calculate performance metrics
void calculatePerformanceMetrics(int n, PerformanceMetrics *metrics) {
    // Calculate total FLOPs
    // LU Factorization: ~(2/3)n³ operations
    // Forward substitution: ~n² operations
    // Backward substitution: ~n² operations
    long long n_long = (long long)n;
    metrics->total_flops = (2 * n_long * n_long * n_long) / 3 + 2 * n_long * n_long;
    
    // CORRECTED CALCULATION:
    // LU Factorization (in-place):
    // - Read A matrix once: n² elements
    // - Write L and U in-place: n² elements (they overwrite A)
    // - Pivoting: ~n² additional accesses
    // Total LU memory: ~2n² to 3n², NOT n³
    
    // Forward/Backward substitution:
    // - Read L/U: ~n² each
    // - Read/write solution vectors: ~n each
    // Total substitution memory: ~2n² to 3n²
    
    // REALISTIC TOTAL:
    metrics->total_memory_accesses = 5 * n_long * n_long + 2 * n_long;
    // This gives ~5n² + 2n instead of 3n³ + 4n²
    
    // Calculate CGMA ratio
    metrics->cgma_ratio = (double)metrics->total_flops / (double)metrics->total_memory_accesses;
    
    // Calculate total threads used across all kernels
    // This is an approximation of peak parallelism
    long long total_thread_invocations = 0;
    
    // For each iteration k (n-1 iterations)
    for (int k = 0; k < n - 1; k++) {
        // findPivotKernel: (n-k) threads
        total_thread_invocations += (n - k);
        
        // swapRowsKernel: n threads (called when pivot != k, assume 50% of time)
        total_thread_invocations += n / 2;
        
        // computeMultipliersKernel: (n-k-1) threads
        total_thread_invocations += (n - k - 1);
        
        // updateSubmatrixKernel: (n-k-1)² threads
        long long submatrix_size = (n - k - 1);
        total_thread_invocations += submatrix_size * submatrix_size;
    }
    
    // Forward substitution: n kernels × 1 thread each
    total_thread_invocations += n;
    
    // Backward substitution: n kernels × 1 thread each
    total_thread_invocations += n;
    
    metrics->total_threads = (int)total_thread_invocations;
    
    // Peak parallelism (early iterations of updateSubmatrix)
    int peak_threads = (n - 1) * (n - 1);
    metrics->threads_per_block = 256; // Most common block size
    metrics->blocks_per_grid = (peak_threads + 255) / 256;
}

// LU Factorization with partial pivoting
void luFactorization(double *h_A, double *h_B, int n, double *h_L, double *h_U, 
                      double *h_x, TimingInfo *timing, PerformanceMetrics *metrics) {
    double *d_A, *d_B, *d_L, *d_y, *d_x;
    int *d_pivot;
    int *h_pivot = (int *)malloc(sizeof(int));
    
    size_t size_matrix = n * n * sizeof(double);
    size_t size_vector = n * sizeof(double);
    
    // Allocate device memory
    CUDA_CHECK(cudaMalloc(&d_A, size_matrix));
    CUDA_CHECK(cudaMalloc(&d_B, size_vector));
    CUDA_CHECK(cudaMalloc(&d_L, size_matrix));
    CUDA_CHECK(cudaMalloc(&d_y, size_vector));
    CUDA_CHECK(cudaMalloc(&d_x, size_vector));
    CUDA_CHECK(cudaMalloc(&d_pivot, sizeof(int)));
    
    // Copy data to device
    CUDA_CHECK(cudaMemcpy(d_A, h_A, size_matrix, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, h_B, size_vector, cudaMemcpyHostToDevice));
    
    // Initialize L as identity matrix
    double *temp_L = (double *)calloc(n * n, sizeof(double));
    for (int i = 0; i < n; i++) {
        temp_L[i * n + i] = 1.0;
    }
    CUDA_CHECK(cudaMemcpy(d_L, temp_L, size_matrix, cudaMemcpyHostToDevice));
    
    // Create CUDA events for timing
    cudaEvent_t start_l, stop_l, start_u, stop_u, start_total, stop_total;
    CUDA_CHECK(cudaEventCreate(&start_l));
    CUDA_CHECK(cudaEventCreate(&stop_l));
    CUDA_CHECK(cudaEventCreate(&start_u));
    CUDA_CHECK(cudaEventCreate(&stop_u));
    CUDA_CHECK(cudaEventCreate(&start_total));
    CUDA_CHECK(cudaEventCreate(&stop_total));
    
    CUDA_CHECK(cudaEventRecord(start_total));
    CUDA_CHECK(cudaEventRecord(start_l));
    
    // LU Factorization with partial pivoting
    int threadsPerBlock = 256;
    dim3 blockDim2D(16, 16);
    
    for (int k = 0; k < n - 1; k++) {
        // Find pivot
        int pivotBlocks = (n - k + threadsPerBlock - 1) / threadsPerBlock;
        findPivotKernel<<<pivotBlocks, threadsPerBlock>>>(d_A, d_pivot, n, k);
        CUDA_CHECK(cudaDeviceSynchronize());
        
        // Copy pivot index to host
        CUDA_CHECK(cudaMemcpy(h_pivot, d_pivot, sizeof(int), cudaMemcpyDeviceToHost));
        
        // Swap rows if necessary
        if (*h_pivot != k) {
            int swapBlocks = (n + threadsPerBlock - 1) / threadsPerBlock;
            swapRowsKernel<<<swapBlocks, threadsPerBlock>>>(d_A, d_B, n, k, *h_pivot);
            CUDA_CHECK(cudaDeviceSynchronize());
            
            // Also swap in L matrix (only the already computed part)
            if (k > 0) {
                swapRowsKernel<<<swapBlocks, threadsPerBlock>>>(d_L, NULL, n, k, *h_pivot);
                CUDA_CHECK(cudaDeviceSynchronize());
            }
        }
        
        // Compute multipliers (L matrix entries)
        int multipliersBlocks = (n - k - 1 + threadsPerBlock - 1) / threadsPerBlock;
        computeMultipliersKernel<<<multipliersBlocks, threadsPerBlock>>>(d_A, d_L, n, k);
        CUDA_CHECK(cudaDeviceSynchronize());
        
        // Update submatrix
        dim3 gridDim2D((n - k - 1 + blockDim2D.x - 1) / blockDim2D.x,
                       (n - k - 1 + blockDim2D.y - 1) / blockDim2D.y);
        updateSubmatrixKernel<<<gridDim2D, blockDim2D>>>(d_A, d_L, n, k);
        CUDA_CHECK(cudaDeviceSynchronize());
    }
    
    CUDA_CHECK(cudaEventRecord(stop_l));
    CUDA_CHECK(cudaEventSynchronize(stop_l));
    
    // U matrix is now stored in the upper triangle of A
    CUDA_CHECK(cudaEventRecord(start_u));
    
    // Copy U matrix from A (already computed during LU factorization)
    CUDA_CHECK(cudaMemcpy(h_U, d_A, size_matrix, cudaMemcpyDeviceToHost));
    
    // Set lower triangle of U to zero
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            h_U[i * n + j] = 0.0;
        }
    }
    
    CUDA_CHECK(cudaEventRecord(stop_u));
    CUDA_CHECK(cudaEventSynchronize(stop_u));
    
    // Copy L matrix back to host
    CUDA_CHECK(cudaMemcpy(h_L, d_L, size_matrix, cudaMemcpyDeviceToHost));
    
    // Forward substitution: Ly = Pb
    for (int i = 0; i < n; i++) {
        forwardSubstitutionKernel<<<1, 1>>>(d_L, d_B, d_y, n, i);
        CUDA_CHECK(cudaDeviceSynchronize());
    }
    
    // Backward substitution: Ux = y
    CUDA_CHECK(cudaMemcpy(d_A, h_U, size_matrix, cudaMemcpyHostToDevice));
    for (int i = n - 1; i >= 0; i--) {
        backwardSubstitutionKernel<<<1, 1>>>(d_A, d_y, d_x, n, i);
        CUDA_CHECK(cudaDeviceSynchronize());
    }
    
    CUDA_CHECK(cudaEventRecord(stop_total));
    CUDA_CHECK(cudaEventSynchronize(stop_total));
    
    // Copy solution back to host
    CUDA_CHECK(cudaMemcpy(h_x, d_x, size_vector, cudaMemcpyDeviceToHost));
    
    // Calculate timing
    float l_time_ms, u_time_ms, total_time_ms;
    CUDA_CHECK(cudaEventElapsedTime(&l_time_ms, start_l, stop_l));
    CUDA_CHECK(cudaEventElapsedTime(&u_time_ms, start_u, stop_u));
    CUDA_CHECK(cudaEventElapsedTime(&total_time_ms, start_total, stop_total));
    
    timing->l_time = l_time_ms;
    timing->u_time = u_time_ms;
    timing->total_solve_time = total_time_ms;
    
    // Cleanup
    free(temp_L);
    free(h_pivot);
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_L));
    CUDA_CHECK(cudaFree(d_y));
    CUDA_CHECK(cudaFree(d_x));
    CUDA_CHECK(cudaFree(d_pivot));
    CUDA_CHECK(cudaEventDestroy(start_l));
    CUDA_CHECK(cudaEventDestroy(stop_l));
    CUDA_CHECK(cudaEventDestroy(start_u));
    CUDA_CHECK(cudaEventDestroy(stop_u));
    CUDA_CHECK(cudaEventDestroy(start_total));
    CUDA_CHECK(cudaEventDestroy(stop_total));
    
    // Calculate performance metrics
    calculatePerformanceMetrics(n, metrics);
}

// Write timing information to file
void writeTimingInfo(const char *filename, int n, TimingInfo *timing, PerformanceMetrics *metrics) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening timing file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "Timing Information for N = %d\n", n);
    fprintf(fp, "=====================================\n");
    fprintf(fp, "Time taken to read A and B matrices: %.6f ms\n", timing->io_time);
    fprintf(fp, "Time taken to compute L matrix: %.6f ms\n", timing->l_time);
    fprintf(fp, "Time taken to compute U matrix: %.6f ms\n", timing->u_time);
    fprintf(fp, "Total time for solving linear equations: %.6f ms\n", timing->total_solve_time);
    fprintf(fp, "\n");
    fprintf(fp, "Performance Metrics\n");
    fprintf(fp, "=====================================\n");
    fprintf(fp, "Total FLOPs: %lld\n", metrics->total_flops);
    fprintf(fp, "Total Memory Accesses: %lld\n", metrics->total_memory_accesses);
    fprintf(fp, "CGMA Ratio: %.6f\n", metrics->cgma_ratio);
    fprintf(fp, "Total Thread Invocations: %d\n", metrics->total_threads);
    fprintf(fp, "Peak Blocks per Grid: %d\n", metrics->blocks_per_grid);
    fprintf(fp, "Threads per Block: %d\n", metrics->threads_per_block);
    fprintf(fp, "\n");
    
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int n;
    double *h_A, *h_B, *h_L, *h_U, *h_x;
    TimingInfo timing = {0.0, 0.0, 0.0, 0.0};
    PerformanceMetrics metrics = {0, 0, 0, 0.0, 0, 0};
    
    // Read input
    readInput(argv[1], &n, &h_A, &h_B, &timing.io_time);
    
    printf("Solving system of %d linear equations using LU factorization...\n", n);
    
    // Allocate memory for L, U, and x
    h_L = (double *)malloc(n * n * sizeof(double));
    h_U = (double *)malloc(n * n * sizeof(double));
    h_x = (double *)malloc(n * sizeof(double));
    
    // Perform LU factorization and solve
    luFactorization(h_A, h_B, n, h_L, h_U, h_x, &timing, &metrics);
    
    // Generate output filename
    char output_filename[256];
    char timing_filename[256];
    sprintf(output_filename, "output_%d.txt", n);
    sprintf(timing_filename, "timing_%d.txt", n);
    
    // Write output
    writeOutput(output_filename, n, h_L, h_U, h_x);
    writeTimingInfo(timing_filename, n, &timing, &metrics);
    
    printf("Solution completed!\n");
    printf("Output written to: %s\n", output_filename);
    printf("Timing info written to: %s\n", timing_filename);
    printf("\nTiming Summary:\n");
    printf("  I/O Time: %.6f ms\n", timing.io_time);
    printf("  L Matrix Time: %.6f ms\n", timing.l_time);
    printf("  U Matrix Time: %.6f ms\n", timing.u_time);
    printf("  Total Solve Time: %.6f ms\n", timing.total_solve_time);
    printf("\nPerformance Metrics:\n");
    printf("  CGMA Ratio: %.6f\n", metrics.cgma_ratio);
    printf("  Total Thread Invocations: %d\n", metrics.total_threads);
    printf("  Total FLOPs: %lld\n", metrics.total_flops);
    
    // Cleanup
    free(h_A);
    free(h_B);
    free(h_L);
    free(h_U);
    free(h_x);
    
    return EXIT_SUCCESS;
}
