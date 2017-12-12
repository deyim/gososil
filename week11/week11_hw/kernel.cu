
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

//1 exercise
//2 prac 1-3
//3 Fibonacci
//4 gen
//5 hw1
#define prac 2


#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1
#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

/*
if block size is 8,					/ gpu: 13.84512
if block size is 16,				/ gpu: 8.617824
if block size is 24,				/ gpu: 9.498592
if block size is 32, cpu: 79.871399 / gpu: 9.26400
*/

#if prac==1

typedef struct {
	int width;
	int height;
	float *elements;
} Array;


#define MAX_N_ELEMENTS	(1 << 25)

void generate_random_float_array(float *array, int n) {

	int i;

	for (i = 0; i < n; i++) {
		array[i] = 3.1415926f*((float)rand() / RAND_MAX);
	}
}
void combine_two_arrays(float *x, float *y, float *z, int n) {
	int i;

	for (i = 0; i < n; i++) {
		z[i] = 1.0f / (sin(x[i])*cos(y[i]) + cos(x[i])*sin(y[i]));
	}
}

__global__ void CombineTwoArrraysKernel(Array A, Array B, Array C) {
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;
	C.elements[id] = 1.0f / (sin(A.elements[id])*cos(B.elements[id])+ cos(A.elements[id])*sin(B.elements[id]));
}

cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C);

int BLOCK_SIZE = 32;

int main()
{
	int n_elements;

	srand((unsigned int)time(NULL));
	n_elements = MAX_N_ELEMENTS;
	Array A, B, C, G;
	A.width = B.width = C.width = G.width =1024;
	A.height = B.height = C.height = G.height = MAX_N_ELEMENTS / 1024;

	A.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	B.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	C.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	G.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	generate_random_float_array(A.elements, MAX_N_ELEMENTS);
	generate_random_float_array(B.elements, MAX_N_ELEMENTS);
	
	CHECK_TIME_START;
	combine_two_arrays(A.elements, B.elements, C.elements, n_elements);
	CHECK_TIME_END(compute_time);

	printf("***CPU C[10] = %f/ Time taken = %.6fms\n", C.elements[10], compute_time);

	//CHECK_TIME_START;
	cudaError_t cudaStatus = combine_two_arrays_GPU(A, B, G);
	//CHECK_TIME_END(compute_time);
	if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "combine_two_arrays_GPU failed!");
        return 1;
    }
	printf("***GPU G[10] = %f/ Time taken = %.6fms\n", G.elements[10], device_time);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}
cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C) {
	
	//아래 함수들을 사용하여 어떻게 하면 가급적 정확한 시간을 측정할 수 있을지 생각해볼 것.
	CHECK_TIME_INIT_GPU()
	
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL
	
	Array d_A, d_B, d_C;
	size_t size;

	d_A.width = A.width; d_A.height = A.height;
	size = A.width * A.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_A.elements, size))
	
	CUDA_CALL(cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice))

	d_B.width = B.width; d_B.height = B.height;
	size = B.width * B.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_B.elements, size))
	CUDA_CALL(cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice))

	d_C.width = C.width; d_C.height = C.height;
	size = C.width * C.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_C.elements, size))

	// Assume that width and height are multiples of BLOCK SIZE.
	
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(A.width / dimBlock.x, A.height / dimBlock.y);
	CHECK_TIME_START_GPU()
	CombineTwoArrraysKernel <<< dimGrid, dimBlock >>> (d_A, d_B, d_C);
	CHECK_TIME_END_GPU(device_time)

	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	CUDA_CALL(cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost))
	

	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
	return cudaStatus;
}
#endif


//N = 4096
//CPU: 11.082584
//Block Size 32 -> 3.419040
//Block Size 24 -> 0.3448
//Block Size 16 -> 0.371936
//Block Size 8 -> 0.369760
#if prac==2
typedef struct {
	int width;
	int height;
	float *elements;
} Array;

int n;
#define BLOCK_SIZE 16
const int ELEM_PER_VECTOR = 32;
float (*pVecX)[ELEM_PER_VECTOR], (*pVecY)[ELEM_PER_VECTOR], (*pVecY_G)[ELEM_PER_VECTOR];
float(*pMatA)[ELEM_PER_VECTOR];

void init_MatVec(void)
{
	srand((unsigned)time(NULL));
	FILE* fp = fopen("gen.bin", "rb");
	fread(&n, sizeof(float), 1, fp);
	printf("n: %d\n", n);
	pVecX = new float[n][ELEM_PER_VECTOR];
	pVecY = new float[n][ELEM_PER_VECTOR];
	pVecY_G = new float[n][ELEM_PER_VECTOR];
	pMatA = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];

	fread(pVecX, sizeof(float), n * ELEM_PER_VECTOR, fp);
	fread(pMatA, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);
	fclose(fp);
}
void Mat_Vec_Multiply()
{
	int i, j, k;
	float sum;
	for( k = 0 ; k < n; k++){
		for( i = 0 ; i < ELEM_PER_VECTOR ; i++){
			sum = 0;
			for(j = 0 ; j < ELEM_PER_VECTOR ; j++){
				sum += pMatA[i][j] * pVecX[k][j];
			}
			pVecY[k][i] = sum;
		}
	}
}

__global__ void Mat_Vec_Multiply_Kernel(const Array VecX, const Array MatA, Array VecY)
{ 
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;
	int id = gridDim.x*blockDim.x*row + col;
	int i,j,k;


	VecY.elements[row*ELEM_PER_VECTOR + col] = 0;
	for(k = 0 ; k < ELEM_PER_VECTOR ; k++)
		VecY.elements[row*ELEM_PER_VECTOR + col] += (MatA.elements[col * ELEM_PER_VECTOR + k] * VecX.elements[row*ELEM_PER_VECTOR+k]);
	
}

void Mat_Vec_Multiply_GPU()
{
	//아래 함수들을 사용하여 어떻게 하면 가급적 정확한 시간을 측정할 수 있을지 생각해볼 것.
	CHECK_TIME_INIT_GPU()
	
	
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL
	
	Array _pVecX, _pMatA, _pVecY_G;
	size_t size;



 	_pVecX.width = ELEM_PER_VECTOR; _pVecX.height = n;
	size = n * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&_pVecX.elements, size))	
	CUDA_CALL(cudaMemcpy(_pVecX.elements, pVecX, size, cudaMemcpyHostToDevice))

	_pMatA.width = ELEM_PER_VECTOR; _pVecX.height = ELEM_PER_VECTOR;
	size = ELEM_PER_VECTOR * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&_pMatA.elements, size))	
	CUDA_CALL(cudaMemcpy(_pMatA.elements, pMatA, size, cudaMemcpyHostToDevice))

	_pVecY_G.width = ELEM_PER_VECTOR; _pVecY_G.height = n;
	size = n * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&_pVecY_G.elements, n*ELEM_PER_VECTOR))

	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	//dim3 dimGrid((ELEM_PER_VECTOR *ELEM_PER_VECTOR )/ dimBlock.x, n / dimBlock.y);
	dim3 dimGrid(_pVecX.width/ dimBlock.x, _pVecX.height / dimBlock.y); //32 n
	CHECK_TIME_START_GPU()
	Mat_Vec_Multiply_Kernel <<< dimGrid, dimBlock >>> (_pVecX, _pMatA, _pVecY_G);
	CHECK_TIME_END_GPU(device_time)
	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())


	CUDA_CALL(cudaMemcpy(pVecY_G, _pVecY_G.elements, n*ELEM_PER_VECTOR, cudaMemcpyDeviceToHost))
	

	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(_pVecX.elements);
	cudaFree(_pMatA.elements);
	cudaFree(_pVecY_G.elements);

}

int main()
{
	init_MatVec();
	printf("n = %d  file open ok.\n", n);

	CHECK_TIME_START;
	Mat_Vec_Multiply();
	CHECK_TIME_END(compute_time);
	printf("***CPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY[0][0], compute_time);
	Mat_Vec_Multiply_GPU();
	printf("***GPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY_G[0][0], device_time);
		
}


#endif

#if prac==3
#define BLOCK_SIZE 8
#define N 67108864 // 8192 * 8192 = 2^13 * 2^13

int Fibonacci(int n) {
	// DO NOT MODIFY THIS FUNCTION!!!
	float sqrt_5, x_0, x_1;
	float tmp_0, tmp_1;

	sqrt_5 = sqrtf(5.0f);
	x_0 = (1.0f + sqrt_5) / 2.0f;
	x_1 = (1.0f - sqrt_5) / 2.0f;

	tmp_0 = tmp_1 = 1.0f;
	for (int i = 0; i < n; i++) {
		tmp_0 *= x_0;
		tmp_1 *= x_1;
	}
	return (int)((tmp_0 - tmp_1) / sqrt_5 + 0.5);
}
void generate_input(int *x, int n) {
	// DO NOT MODIFY THIS FUNCTION!!!
	srand((unsigned int)time(NULL));

	for (int i = 0; i < n; i++) {
		x[i] = 35 + (int)(5.0f * rand() / RAND_MAX + 0.5f);
	}
}
__global__ void Fibonacci_Kernel(int *x, int *y)
{
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;
	//int id = threadIdx.x;
	float sqrt_5, x_0, x_1;
	float tmp_0, tmp_1;

	sqrt_5 = sqrtf(5.0f);
	x_0 = (1.0f + sqrt_5) / 2.0f;
	x_1 = (1.0f - sqrt_5) / 2.0f;

	tmp_0 = tmp_1 = 1.0f;
	for (int i = 0; i < x[id]; i++) {
		tmp_0 *= x_0;
		tmp_1 *= x_1;
	}
	y[id] = (int)((tmp_0 - tmp_1) / sqrt_5 + 0.5);
}
void Fibonacci_GPU(int *x, int *y)
{
	CHECK_TIME_INIT_GPU()
	cudaError_t cudaStatus;
		// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}

	int *_y, *_x;
	size_t size = N * sizeof(int);
	size_t rtsize = (size_t)(sqrt((float)N));

	CUDA_CALL(cudaMalloc(&_y, size))

	CUDA_CALL(cudaMalloc(&_x, size))	
	CUDA_CALL(cudaMemcpy(_x, x, size, cudaMemcpyHostToDevice))	

	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	//dim3 dimGrid( rtsize / dimBlock.x,  rtsize / dimBlock.y);
	dim3 dimGrid( 1024 / dimBlock.x,  (N/1024) / dimBlock.y);
	CHECK_TIME_START_GPU()
	Fibonacci_Kernel <<< dimGrid, dimBlock >>> (_x, _y);
	CHECK_TIME_END_GPU(device_time)
	CUDA_CALL(cudaGetLastError())

	CUDA_CALL(cudaDeviceSynchronize())

	CUDA_CALL(cudaMemcpy(y, _y, size, cudaMemcpyDeviceToHost))
	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(_x);
	cudaFree(_y);

}

int *x, *y_c, *y_g; // input/output arrays

void main(void) {
	
	int n, i;
	FILE *fp;

	// Read the input array from the input file if one already exists.
	fp = fopen("x.binary", "rb");
	if (!fp) {
		fprintf(stderr, "Error: cannot open the input file...\n");
		exit(-1);
	}
	fread(&n, sizeof(int), 1, fp);
	fprintf(stdout, "\n*** The problem size is %d.\n", n);

	x = (int *)malloc(sizeof(int)*n);
	if (!x) {
		fprintf(stderr, "Error: cannot allocate memory for the input array...\n");
		exit(-1);
	}
	fread(x, sizeof(int), n, fp);
	fclose(fp);

	y_c = (int *)malloc(sizeof(int)*n); 
	y_g = (int *)malloc(sizeof(int)*n);

	//CPU
	
	CHECK_TIME_START;
	for (i = 0; i < n; i++) {
		y_c[i] = Fibonacci(x[i]);
	}
	CHECK_TIME_END(compute_time);
	fprintf(stdout, "\n***_CPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, compute_time);
	
	//GPU
	Fibonacci_GPU(x, y_g);

	fprintf(stdout, "\n***_GPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, device_time);

	i = (int)(n * (rand() / (RAND_MAX + 1.0f)));
	fprintf(stdout, "*** Fibonacci number of %d is (CPU :%d  , GPU :%d).\n\n", x[i], y_c[i], y_g[i]);

	// Write the output array into the output file.
	fp = fopen("y.binary", "wb");
	if (!fp) {
		fprintf(stderr, "Error: cannot open the output file...\n");
		exit(-1);
	}
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(y_c, sizeof(int), n, fp);
	fclose(fp);

	free(x);
	free(y_c);
	free(y_g);
}


#endif

#if prac==4

const int ELEM_PER_VECTOR = 32;

int main()
{
	int n;

	srand((unsigned)&n);

	printf("Enter a size: ");
	scanf("%d", &n);

	int size = ELEM_PER_VECTOR * n;
	float* vec = new float[size];

	for (int i = 0; i < size; ++i)
	{
		vec[i] = (float(rand()) * 2.f / RAND_MAX) - 1.f;
	}

	float(*mat)[ELEM_PER_VECTOR] = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];
	for (int i = 0; i < ELEM_PER_VECTOR; ++i)
	{
		for (int j = 0; j < ELEM_PER_VECTOR; ++j)
		{
			mat[i][j] = (float(rand()) * 2.f / RAND_MAX) - 1.f;
		}
	}

	FILE* fp = fopen("gen.bin", "wb");
	fwrite(&n, sizeof(float), 1, fp);
	fwrite(vec, sizeof(float), size, fp);
	fwrite(mat, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);
	fclose(fp);

	fp = fopen("gen.bin", "rb");
	float* vec2 = new float[size];
	float(*mat2)[ELEM_PER_VECTOR] = new float[ELEM_PER_VECTOR][ELEM_PER_VECTOR];
	int m;
	fread(&m, sizeof(float), 1, fp);
	fread(vec2, sizeof(float), m * ELEM_PER_VECTOR, fp);
	fread(mat2, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);

	if (n != m) printf("error: size diff. %n != %n", n, m);
	for (int i = 0; i < size; ++i)
	{
		if (vec[i] != vec2[i])
		{
			printf("[%d] %f != %f\n", vec[i], vec2[i]);
			break;
		}
	}
	for (int i = 0; i < ELEM_PER_VECTOR; ++i)
	{
		for (int j = 0; j < ELEM_PER_VECTOR; ++j)
		{
			if (mat[i][j] != mat2[i][j])
			{
				printf("[%d][%d]\n", i, j);
				break;
			}
		}
	}
	fclose(fp);

	delete[] vec;

	return 0;
}

#endif


#if prac==5

#define N_EQUATIONS 1048576
#define BLOCK_SIZE 16
float *A, *B, *C;

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
	
	float a, b, c, d, x0, x1, tmp;

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
	size_t size = n*sizeof(float);
	size_t rtsize = (size_t)sqrt(float(n));


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
	//dim3 dimGrid( rtsize/ dimBlock.x, rtsize / dimBlock.y);
	dim3 dimGrid( 512/ dimBlock.x, 2048 / dimBlock.y);

	CHECK_TIME_START_GPU()
	find_roots_Kernel <<< dimGrid, dimBlock >>> (_A, _B, _C, _X0, _X1, _FX0, _FX1, n);
	CHECK_TIME_END_GPU(device_time)
	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	

	CUDA_CALL(cudaMemcpy(X0, _X0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(X1, _X1, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX0, _FX0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX1, _FX1, size, cudaMemcpyDeviceToHost))

	
	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(_A);
	cudaFree(_B);
	cudaFree(_C);
	cudaFree(_X0);
	cudaFree(_X1);
	cudaFree(_FX0);
	cudaFree(_FX1);
}

void read_poly(){
	//float *_A, float *_B, float *_C
	int i, n = N_EQUATIONS;

	
	FILE *fA = fopen("A.bin", "rb");
	if(!fA){
		printf("file open error\n");
		exit(-1);
	}	
	fread(&n, sizeof(float), 1, fA);
	A = (float*)malloc(sizeof(float)*N_EQUATIONS);
	fread(A, sizeof(float), N_EQUATIONS, fA);
	fclose(fA);
	

	FILE *fB = fopen("B.bin", "rb");
	if(!fB){
		printf("file open error\n");
		exit(-1);
	}
	fread(&n, sizeof(float), 1, fB);
	B = (float*)malloc(sizeof(float)*N_EQUATIONS);
	fread(B, sizeof(float), n, fB);
	fclose(fB);

	FILE *fC = fopen("C.bin", "rb");
	if(!fC){
		printf("file open error\n");
		exit(-1);
	}
	fread(&n, sizeof(float), 1, fC);
	C = (float*)malloc(sizeof(float)*N_EQUATIONS);
	fread(C, sizeof(float), n, fC);
	fclose(fC);
}

void write_poly(float *X0, float *X1, float *FX0, float *FX1, int n)
{
	FILE *x0, *x1, *fx0, *fx1;
	size_t cnt;

	x0 = fopen("X0.bin", "wb");
	cnt = fwrite (X0, 4, n, x0);
	fclose(x0);

	x1 = fopen("X1.bin", "wb");
	cnt = fwrite (X1, 4, n, x1);
	fclose(x1);
	
	fx0 = fopen("FX0.bin", "wb");
	cnt = fwrite (FX0, 4, n, fx0);
	fclose(fx0);

	fx1 = fopen("FX1.bin", "wb");
	cnt = fwrite (FX1, 4, n, fx1);
	fclose(fx1);

}

int main(){
//	float *A, *B, *C;
	float *cX0, *cX1, *cFX0, *cFX1, *gX0, *gX1, *gFX0, *gFX1;
	int n = N_EQUATIONS;

	//read files
	read_poly();
	printf("n = %d  file open ok.\n", n);

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
	printf("CPU result X0[1048575], fX0[1048575] = %f, %f\n", cX0[1048575], cFX0[1048575]);
	printf("GPU result X0[1048575], fX0[1048575] = %f, %f\n", gX0[1048575], gFX0[1048575]);

	//save in file
	write_poly(gX0, gX1, gFX0, gFX1, n);

}


#endif














