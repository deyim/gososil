#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void ansFormula1(double, double, double, double*, double*);
void ansFormula2(double, double, double, double*, double*);

int main()
{
	double a,b,c;
	double ans1, ans2;
	printf("scanf numbers\n");
	scanf("%lf%lf%lf", &a, &b, &c);

	if(a ==0){
		printf("a cannot be zero re enter\n");
		scanf("%lf%lf%lf", &a, &b, &c);
	} 

	printf("Formula: %lfx^2 + %lfx + %lf\n\n", a, b, c);
	ansFormula1(a,b,c, &ans1, &ans2);
	ansFormula2(a,b,c, &ans1, &ans2);
 }

void ansFormula1(double a, double b, double c, double *ans1, double *ans2)
{
	double quad, chk, _ans1, _ans2;
	quad = (b*b) - (4*a*c);
	quad = sqrt(quad);
	*ans1 = ((-1)*b + quad) / (2*a);
	*ans2 = ((-1)*b - quad) / (2*a);
	printf("solved with formula 1\n");
	//printf("ans1: %.10lf, ans2: %.10lf\n", *ans1, *ans2);
	printf("ans1: %.10e, ans2: %.10e\n", *ans1, *ans2);
	_ans1 = *ans1; _ans2 = *ans2;
	chk = (_ans1 * _ans1) * a + _ans1 * b + c;
	//printf("check value with ans1: %.10lf\n", chk);
	printf("check value with ans1: %.10e\n", chk);
	chk = (_ans2 * _ans2) * a + _ans2 * b + c;
	//printf("check value with ans2: %.10lf\n\n", chk);
	printf("check value with ans2: %.10e\n\n", chk);
	
}

void ansFormula2(double a, double b, double c, double *ans1, double *ans2)
{
	double quad, chk, _ans1, _ans2;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	if(b>0) *ans1 = ((-2)*c) / (b+quad);
	else 
		*ans1 = ((-1)*b + quad) / (2*a);
	//*ans2 = ((-2)*c) / (-b+quad);
	if(b<0) *ans2 = ((-2)*c) / (b-quad);
	else 
		*ans2 = ((-1)*b - quad) / (2*a);
	printf("solved with formula 2\n");
	//printf("ans1: %.10lf, ans2: %.10lf\n\n", *ans1, *ans2);
	printf("ans1: %.10e, ans2: %.10e\n\n", *ans1, *ans2);
	
	_ans1 = *ans1; _ans2 = *ans2;
	chk = (_ans1*_ans1)*a + _ans1*b + c;
	//printf("check value with ans1: %.10lf\n", chk);
	printf("check value with ans1: %.10e\n", chk);
	chk = (_ans2*_ans2)*a + _ans2*b + c;
	//printf("check value with ans2: %.10lf\n\n", chk);
	printf("check value with ans2: %.10e\n\n", chk);
}













