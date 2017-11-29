#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define N 25
#define X -8.30
double Taylor_series(double x, int n);
double Taylor_series_ex(double x, int n);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

void main(void)
{
	double ans1, ans2;

	//Taylor series function
	ans1 = Taylor_series(X, N);
	printf("***F(%f) = %.10e\n", X, ans1);

	//accurate version of taylor series function
	ans2 = Taylor_series_ex(X, N);
	printf("***F(%f) = %.10e\n", X, ans2);

	ans1=0;
}


double Taylor_series(double x, int n)
{
	int i, fact;
	double ans=1;
	for(i = n ; i > 0 ; i--){ //for factorial number
		ans = ((ans*x)/i)+1; // divide ans*X by i (increase factorial number) + 1
	}
	return ans;
}
double Taylor_series_ex(double x, int n)
{
	int i, fact;
	double target, ans=1;
	if(x <0) //if X is smaller than 0,
		target = x * -1; //calculate 1/e^(-x)
	for(i = n ; i > 0 ; i--){ 
		ans = ((ans*target)/i)+1; //same as taylor series function
	}
	return 1/ans;
}
