#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void ansFormula1(float, float, float, float *, float *);
void ansFormula2(float, float, float, float *, float *);

int main()
{
	float a,b,c;
	float ans1, ans2;
	printf("scanf numbers\n");
	scanf("%f%f%f", &a, &b, &c);

	if(a ==0){
		printf("a cannot be zero re enter\n");
		scanf("%f%f%f", &a, &b, &c);
	} 

	printf("Formula: %fx^2 + %fx + %f\n", a, b, c);
	ansFormula1(a,b,c, &ans1, &ans2);


	ansFormula2(a,b,c, &ans1, &ans2);
 }

void ansFormula1(float a, float b, float c, float *ans1, float *ans2)
{
	float quad, chk, _ans1 = *ans1, _ans2 = *ans2;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	*ans1 = ((-1)*b + quad) / (2*a);
	*ans2 = ((-1)*b - quad) / (2*a);
	printf("solved with formula 1\n");
	printf("ans1: %.10f, ans2: %.10f\n", *ans1, *ans2);
	chk = _ans1*_ans1*a + _ans1*b + c;
	printf("check value with ans1: %.10f\n", chk);
	chk = _ans2*_ans2*a + _ans2*b + c;
	printf("check value with ans2: %.10f\n\n", chk);
	
}

void ansFormula2(float a, float b, float c, float *ans1, float *ans2)
{
	float quad, chk, _ans1 = *ans1, _ans2 = *ans2;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	*ans1 = ((-2)*c) / (b+quad);
	*ans2 = ((-1)*b - quad) / (2*a);
	printf("solved with formula 2\n");
	printf("ans1: %.10f, ans2: %.10f\n\n", *ans1, *ans2);
	chk = _ans1*_ans1*a + _ans1*b + c;
	printf("check value with ans1: %.10f\n", chk);
	chk = _ans2*_ans2*a + _ans2*b + c;
	printf("check value with ans2: %.10f\n\n", chk);
}













