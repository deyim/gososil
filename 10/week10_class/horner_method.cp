#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define DEGREE 10

//#define N_X 1048576
#define N_X 10
void Eval_Poly_Naive(double y[], double x[], int n_x, double a[], int deg);
void Eval_Poly_Horner(double y[], double x[], int n_x, double a[], int deg);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
void main(void)
{
	double *ptr;
	double sum=0;
	double a[DEGREE+1];
	double *x = (double *)malloc(sizeof(double)*N_X);
	double *y_n = (double *)malloc(sizeof(double)*N_X);
	double *y_h = (double *)malloc(sizeof(double)*N_X);

	//assign random number of values
	srand((unsigned)time(NULL));
	ptr = a;
	for (int i = 0; i < DEGREE+1; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = x;
	for (int i = 0; i < N_X; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);

	//Eval_Poly_Naive
	CHECK_TIME_START;
	Eval_Poly_Naive(y_n, x, N_X, a, DEGREE);
	CHECK_TIME_END(compute_time);
	printf("Eval_Poly_Naive= %.10f ms\n", compute_time);

	//Eval_Poly_Horner
	CHECK_TIME_START;
	Eval_Poly_Horner(y_h, x, N_X, a, DEGREE);
	CHECK_TIME_END(compute_time);
	printf("Eval_Poly_Horner= %.10f ms\n", compute_time);

	//error calculation is it small enough?
	for(int i = 0 ; i < N_X ; i++){
		sum += (y_n[i] - y_h[i])*(y_n[i] - y_h[i]);
	}
	//print error and small number of result values
	printf("sum of (yn - yh)^2: %.10f\n", sum);
	for(int i = 0 ; i < 10 ; i++)
		printf("%f\t%f\n", y_n[i], y_h[i]);
}
void Eval_Poly_Naive(double y[], double x[], int n_x, double a[], int deg)
{
	int i,j;
	double sum;
	for(i = 0 ; i < n_x ; i++){ //for x,y index
		sum = 0;
		for(j = 0 ; j< deg ;j++){ // add a[i] * x^i 0<=i<deg
			sum += (a[j] * pow(x[i], j));
		}
		y[i] = sum;
	}

}
void Eval_Poly_Horner(double y[], double x[], int n_x, double a[], int deg)
{
	int i,j;
	double sum;
	printf("\n\n");
	for(i = 0 ; i < n_x ; i++){ //for x,y index
		sum = a[deg];
		for(j = deg ; j > 0 ;j--){ // sum * x + a for all degree
			sum = sum*x[i] + a[j-1];
		}
		y[i] = sum;
	}
	
}
