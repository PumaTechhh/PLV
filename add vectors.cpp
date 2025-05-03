#include <iostream>
#include <cuda_runtime.h>
using namespace std;

// CUDA Kernel for vector addition
__global__ void vectorAdd(int *A, int *B, int *C, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int N = 1000000; // Size of vectors
    size_t size = N * sizeof(int);
    
    // Allocate host memory
    int *h_A = (int*)malloc(size);
    int *h_B = (int*)malloc(size);
    int *h_C = (int*)malloc(size);
    
    // Initialize vectors
    for (int i = 0; i < N; i++) {
        h_A[i] = i;
        h_B[i] = i * 2;
    }
    
    // Allocate device memory
    int *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);
    
    // Copy data to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
    
    // Define block and grid size
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    
    // Launch kernel
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
    
    // Copy result back to host
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    
    // Print sample result
    cout << "Sample result: " << h_C[0] << ", " << h_C[N-1] << endl;
    
    // Free memory
    free(h_A); free(h_B); free(h_C);
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    
    return 0;
}



/////////
1.CUDA Kernel (vectorAdd)

__global__ void vectorAdd(int *A, int *B, int *C, int N) { ... }
	Each thread computes one element of the sum.
2.Memory Allocation

int *d_A, *d_B, *d_C;
cudaMalloc((void**)&d_A, size);
	Allocates memory on GPU (device memory).
3.Memory Transfer (Host ↔ Device)

cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	Copies vectors from CPU to GPU.
4.Kernel Launch

vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
	Uses CUDA parallel execution.
5.Copy Result Back & Free Memory

cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

	Transfers result from GPU to CPU.
 
Compiling and Running the CUDA Program
Compile:

nvcc cuda_vector_addition.cu -o cuda_vector_addition

Run:

./cuda_vector_addition


Output:
//////////////////
