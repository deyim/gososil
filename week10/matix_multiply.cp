#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;


#define MATDIM 1024
double *pMatA, *pMatB, *pMatC;
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);

void init_MatMat(void);
//Sleep(500);
void main(void)
{
	init_MatMat();
	
	//function 1
	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_1= %f ms\n", compute_time);
	
	//function 2
	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_2= %f ms\n", compute_time);

	//function 3
	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_3= %f ms\n", compute_time);

	//function 4
	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_4= %f ms\n", compute_time);

}

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix ,double *pRightMatrix, int MatSize)
{
	int i,j,k,destIdx=0;
	double sum=0;

	for(i = 0 ; i < MatSize ; i++){ //L's row
		for(j=0; j< MatSize ; j++){ //R's col
			sum = 0;
			for(k = 0 ; k < MatSize ; k++){ // L's col and R's row
				sum += pLeftMatrix[i*MatSize+k] * pRightMatrix[k*MatSize+j];
			}
			pDestMatrix[i*MatSize + j] = sum;
		}
		
	}

	
}

void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{	
	int i,j,k,destIdx;
	double *pTranspose, sum=0;
	pTranspose = (double *)malloc(sizeof(double)*MatSize*MatSize);
	
	//Transpose Right Matrix
	for(i = 0 ; i < MatSize ; i++){ 
		for(j=0; j< MatSize ; j++){ 
			pTranspose[j*MatSize + i] = pRightMatrix[i*MatSize+j];
		}
		
	}

	//Multiply
	for(i = 0 ; i < MatSize ; i++){ 
		for(j=0; j< MatSize ; j++){ 
			destIdx = i*MatSize + j;
			pDestMatrix[destIdx] = 0;
			for(k = 0; k < MatSize ; k++){
				pDestMatrix[destIdx] += pLeftMatrix[i*MatSize+k] * pRightMatrix[j*MatSize+k];
			}
		}
	}


}
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
		int i,j,k, leftIdx, rightIdx, destIdx;
	double *pTranspose, sum=0;
	pTranspose = (double *)malloc(sizeof(double)*MatSize*MatSize);
	
	//Transpose Right Matrix
	for(i = 0 ; i < MatSize ; i++){ 
		for(j=0; j< MatSize ; j++){ 
			pTranspose[j*MatSize + i] = pRightMatrix[i*MatSize+j];
		}
	}

	for(i = 0 ; i < MatSize ; i++){ //left row
		for(j=0; j< MatSize ; j++){ // right column (== row for transposed matrix)
			destIdx = i*MatSize + j;
			pDestMatrix[destIdx] = 0;
			for(k = 0; k < MatSize ; k += 10){//left column and right row 
				//for loop unrolling with 30 interval
				leftIdx = i*MatSize+k; rightIdx = j*MatSize+k;
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++]; //10
			}
		}
	}
}
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i,j,k, leftIdx, rightIdx, destIdx;
	double *pTranspose, sum=0;
	pTranspose = (double *)malloc(sizeof(double)*MatSize*MatSize);
	
	//Transpose Right Matrix
	for(i = 0 ; i < MatSize ; i++){ 
		for(j=0; j< MatSize ; j++){ 
			pTranspose[j*MatSize + i] = pRightMatrix[i*MatSize+j];
		}
	}

	for(i = 0 ; i < MatSize ; i++){ //left row
		for(j=0; j< MatSize ; j++){ // right column (== row for transposed matrix)
			destIdx = i*MatSize + j;
			pDestMatrix[destIdx] = 0;
			for(k = 0; k < MatSize ; k += 30){ //left column and right row 
				//for loop unrolling with 30 interval
				leftIdx = i*MatSize+k; rightIdx = j*MatSize+k;
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++]; //10
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
				pDestMatrix[destIdx] += pLeftMatrix[leftIdx++] * pRightMatrix[rightIdx++];
			}
		}
	}
}


void init_MatMat(void)
{
	double *ptr;
	pMatA = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}