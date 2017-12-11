#if prac==3

int *x, *y_c, *y_g; // input/output arrays
#define BLOCK_SIZE 32
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
	size_t size = N;
	size_t rtsize = sqrt(float(n));

	CUDA_CALL(cudaMalloc(&_y, size))

	CUDA_CALL(cudaMalloc(&_x, size))	
	CUDA_CALL(cudaMemcpy(_x, x, size, cudaMemcpyHostToDevice))	

	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid( rtsize / dimBlock.x,  rtsize / dimBlock.y);
	Fibonacci_Kernel <<< dimGrid, dimBlock >>> (_x, _y);

	CUDA_CALL(cudaGetLastError())

	CUDA_CALL(cudaDeviceSynchronize())
	CHECK_TIME_END_GPU(device_time)

	CUDA_CALL(cudaMemcpy(y, _y, size, cudaMemcpyDeviceToHost))
	CHECK_TIME_DEST_GPU()

Error:
	cudaFree(_x);
	cudaFree(_y);
}

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


	//GPU
	Fibonacci_GPU(x, y_g);


	fprintf(stdout, "\n***_CPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, compute_time);

	//fprintf(stdout, "\n***_GPU_ Time taken for computing %d Fibonacci numbers is %.6fms\n\n", n, device_time);

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