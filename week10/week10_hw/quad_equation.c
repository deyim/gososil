#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define N 100

float makeAverage(float data[], int cnt);
float makeVar1(float data[], int cnt);
float makeVar2(float data[], int cnt, float avg);

int main()
{
	//variables declaration
	float *data, avg, var1, var2;
	srand((int)time(NULL));
	//make random number and sample data
	data = (float*)malloc(sizeof(float)*N);
	for(int i = 0 ; i < N ; i++)
		data[i] = ((float)rand())/RAND_MAX*1000;

	//average
	avg = makeAverage(data, N);
	var1 = makeVar1(data, N);
	var2 = makeVar2(data, N, avg);

	printf("avg: %f, var1: %f, var2: %f\n", avg, var1, var2);
 }

float makeAverage(float data[], int cnt)
{
	float total=0;
	for(int i = 0 ; i < cnt ; i++)
		total += data[i];
	return total/cnt;
}

float makeVar1(float data[], int cnt)
{
	float quad = 0, total=0;
	for(int i = 0 ; i < cnt; i++){
		quad += (data[i] * data[i]);
	}
	for(int i = 0 ; i < cnt; i++){
		total += data[i];
	}
	quad *= cnt; total *= total;
	return (quad-total)/(cnt*(cnt-1));
}

float makeVar2(float data[], int cnt, float avg)
{
	float total=0;
	for(int i = 0 ; i < cnt; i++){
		total += (data[i] - avg) * (data[i] - avg);
	}
	return total / (cnt-1);
}


