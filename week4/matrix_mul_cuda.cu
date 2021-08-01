#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include<cuda.h>
#include<cuda_runtime.h>

struct Matrix{
int width;
int height;
float *elements;
};

extern “C” device float getElement(Matrix *A, int row, int col){
return A->elements[row * A->width + col];
}

extern “C” device void setElement(Matrix *A, int row, int col, float value){
A->elements[row * A->width + col] = value;
}

extern “C” global void matMulKernel(Matrix *A, Matrix *B, Matrix *C){
float Cvalue = 0.0;
int row = threadIdx.y + blockIdx.y * blockDim.y;
int col = threadIdx.x + blockIdx.x * blockDim.x;

    for (int i = 0; i < A->width; ++i){
            Cvalue += getElement(A, row, i) * getElement(B, i, col);
    }
    setElement(C, row, col, Cvalue);
}

extern “C” int matrixX(float *h_A,float *h_B,float *h_C_host,int w){
int width = w;
int height = w;
double timeuse;

Matrix *A, *B, *C;
cudaMallocManaged((void**)&A, sizeof(Matrix));
cudaMallocManaged((void**)&B, sizeof(Matrix));
cudaMallocManaged((void**)&C, sizeof(Matrix));
 
int nBytes = width * height * sizeof(float);
 
cudaMallocManaged((void**)&A->elements, nBytes);
cudaMallocManaged((void**)&B->elements, nBytes);
cudaMallocManaged((void**)&C->elements, nBytes);
 
A->height = height;
A->width = width;
B->height = height;
B->width = width;
C->height = height;
C->width = width;

cudaMemcpy(A->elements,h_A,w*w*sizeof(float),cudaMemcpyHostToDevice);
cudaMemcpy(B->elements,h_B,w*w*sizeof(float),cudaMemcpyHostToDevice);
 
dim3 blockSize(16, 16);
dim3 gridSize((width + blockSize.x - 1) / blockSize.x,
    (height + blockSize.y - 1) / blockSize.y);
 
struct timeval t1,t2;
gettimeofday(&t1,NULL);
 
matMulKernel << < gridSize, blockSize >> >(A, B, C);
 
 cudaDeviceSynchronize();//After the kernel function call, cudaDeviceSynchronize() must be called to wait for the device to complete the access before the CPU can access it
 
cudaMemcpy(h_C_host,C->elements,w*w*sizeof(float),cudaMemcpyDeviceToHost);
 
gettimeofday(&t2,NULL);
timeuse = t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
printf("Use Time:%fs\n", timeuse);
cudaFree(A->elements);
cudaFree(B->elements);
cudaFree(C->elements);
cudaFree(A);
cudaFree(B);
cudaFree(C);
 
return 0;
}