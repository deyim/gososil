#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))

#define N_EQUATIONS 2^20
#define BLOCK_SIZE 32
float compute_time;


void find_roots_CPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1, int n) 
{ 
	int i; 
	float a, b, c, d, x0, x1, tmp;

	for (i = 0; i < n; i++) {
		a = A[i]; b = B[i]; c = C[i]; 
		d = sqrtf(b*b - 4.0f*a*c); 
		tmp = 1.0f / (2.0f*a); 
		X0[i] = x0 = (-b - d) * tmp; 
		X1[i] = x1 = (-b + d) * tmp; 
		FX0[i] = (a*x0 + b)*x0 + c; 
		FX1[i] = (a*x1 + b)*x1 + c;
	}
}

__global__ void find_roots_Kernel(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1, int n)
{ 
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;
	int i = gridDim.x*blockDim.x*row + col;
	
	a = A[i]; b = B[i]; c = C[i]; 
	d = sqrtf(b*b - 4.0f*a*c); 
	tmp = 1.0f / (2.0f*a); 
	X0[i] = x0 = (-b - d) * tmp; 
	X1[i] = x1 = (-b + d) * tmp; 
	FX0[i] = (a*x0 + b)*x0 + c; 
	FX1[i] = (a*x1 + b)*x1 + c;
}

void find_roots_GPU(float *A, float *B, float *C, float *X0, float *X1, float *FX0, float *FX1, int n)
{

	CHECK_TIME_INIT_GPU()
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL
	
	//Array _pVecX, _pMatA, _pVecY_G;
	float *_A, *_B, *_C;
	float *_X0, *_X1, *_FX0, *_FX1;
	size_t size = n;
	size_t rtsize = sqrt(float(n));


	CUDA_CALL(cudaMalloc(&_A, size))	
	CUDA_CALL(cudaMemcpy(_A, A, size, cudaMemcpyHostToDevice))
	CUDA_CALL(cudaMalloc(&_B, size))	
	CUDA_CALL(cudaMemcpy(_B, B, size, cudaMemcpyHostToDevice))
	CUDA_CALL(cudaMalloc(&_C, size))	
	CUDA_CALL(cudaMemcpy(_C, C, size, cudaMemcpyHostToDevice))

	CUDA_CALL(cudaMalloc(&_X0, size))
	CUDA_CALL(cudaMalloc(&_X1, size))
	CUDA_CALL(cudaMalloc(&_FX0, size))
	CUDA_CALL(cudaMalloc(&_FX1, size))

	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	//dim3 dimBlock(BLOCK_SIZE);
	//dim3 dimGrid( size / dimBlock.x); 
	dim3 dimGrid( rtsize/ dimBlock.x, rtsize / dimBlock.y);
	//dim3 dimGrid(_pVecX.width/ dimBlock.x, _pVecX.height / dimBlock.y); //32 n
	CHECK_TIME_START_GPU()
	Mat_Vec_Multiply_Kernel <<< dimGrid, dimBlock >>> (_A, _B, _C, _X0, _X1, _FX0, _FX1);

	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	CHECK_TIME_END_GPU(device_time)

	CUDA_CALL(cudaMemcpy(X0, _X0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(X1, _X1, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX0, _FX0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX1, _FX1, size, cudaMemcpyDeviceToHost))

	

	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(_pVecX.elements);
	cudaFree(_pMatA.elements);
	cudaFree(_pVecY_G.elements);

}

void read_poly(float *A, float *B, float *C, int *n){
	FILE *fA, *fB, *fC;
	int i;

	fA = fopen("A.bin", "r");
	if(!fA){
		printf("file open error\n");
		exit(-1);
	}
	fscanf(fA, "%f", n);
	A = (float*)malloc(sizeof(float)*n);
	for(i = 0 ; i < n ; i++){
		fscanf(fA, "%f", &A[i]);
	}
	fclose(fA);

	fB = fopen("B.bin", "r");
	if(!fB){
		printf("file open error\n");
		exit(-1);
	}
	fscanf(fA, "%f", n);
	B = (float*)malloc(sizeof(float)*n);
	for(i = 0 ; i < n ; i++){
		fscanf(fA, "%f", &B[i]);
	}
	fclose(fB);

	fC = fopen("C.bin", "r");
	if(!fA){
		printf("file open error\n");
		exit(-1);
	}
	fscanf(fC, "%f", n);
	C = (float*)malloc(sizeof(float)*n);
	for(i = 0 ; i < n ; i++){
		fscanf(fC, "%f", &C[i]);
	}
	fclose(fC);
}

void write_poly(float *X0. float *X1, float *FX0, float FX1, int n)
{
	FILE *x0, *x1, *fx0, *fx1;

	x0 = fopen("X0.bin", "w");
	for(int i = 0 ; i < n; i++) fprintf(x0, "%f", X0[i]);
	fclose(x0);

	x1 = fopen("X1.bin", "w");
	for(int i = 0 ; i < n; i++) fprintf(x0, "%f", X1[i]);
	fclose(x1);
	
	fx0 = fopen("FX0.bin", "w");
	for(int i = 0 ; i < n; i++) fprintf(x0, "%f", FX0[i]);
	fclose(fx0);

	fx1 = fopen("FX1.bin", "w");
	for(int i = 0 ; i < n; i++) fprintf(x0, "%f", FX1[i]);
	fclose(fx1);

}

int main(){
	float *A, *B, *C;
	float *cX0, *cX1, *cFX0, *cFX1, *gX0, *gX1, *gFX0, *gFX1;
	int n;

	//read files
	read_poly(A,B,C, &n);
	printf("n = %d  file open ok.\n", N_EQUATIONS);

	//check CPU time
	cX0 = (float*)malloc(sizeof(float)*n);
	cX1 = (float*)malloc(sizeof(float)*n);
	cFX0 = (float*)malloc(sizeof(float)*n);
	cFX1 = (float*)malloc(sizeof(float)*n);
	CHECK_TIME_START;
	find_roots_CPU(A,B,C, cX0, cX1, cFX0, cFX1, n);
	CHECK_TIME_END(compute_time);
	printf("***CPU Time taken = %.6fms\n", compute_time);
	
	//check GPU time
	gX0 = (float*)malloc(sizeof(float)*n);
	gX1 = (float*)malloc(sizeof(float)*n);
	gFX0 = (float*)malloc(sizeof(float)*n);
	gFX1 = (float*)malloc(sizeof(float)*n);
	find_roots_GPU(A,B,C, gX0, gX1, gFX0, gFX1, n);
	printf("***GPU Time taken = %.6fms\n", device_time);

	//check if same result
	printf("CPU result X0[0], fX0[0] = %f, %f\n", cX0[0], cFX[0]);
	printf("GPU result X0[0], fX0[0] = %f, %f\n", gX0[0], gFX[0]);

	//save in file
	write_poly(gX0, gX1, gFX0, gFX1, n);

}










