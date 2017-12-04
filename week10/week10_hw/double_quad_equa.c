#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void ansFormula1(double, double, double, double*, double*);
void ansFormula2(double, double, double, double*, double*);
//void ansFormula1(float, float, float, float *, float *);
//void ansFormula2(float, float, float, float *, float *);

int main()
{
	//float a,b,c;
	//float ans1, ans2;
	double a,b,c;
	double ans1, ans2;
	printf("scanf numbers\n");
	scanf("%lf%lf%lf", &a, &b, &c);

	if(a ==0){
		printf("a cannot be zero re enter\n");
		scanf("%lf%lf%lf", &a, &b, &c);
	} 

	printf("\nFormula: %lfx^2 + %lfx + %lf\n", a, b, c);
	ansFormula1(a,b,c, &ans1, &ans2);


	ansFormula2(a,b,c, &ans1, &ans2);
 }

///void ansFormula1(float a, float b, float c, float *ans1, float *ans2)
void ansFormula1(double a, double b, double c, double* ans1, double* ans2)
//void ansFormula2(double, double, double, double*, double*);
{
	double quad, chk, _ans1 = *ans1, _ans2 = *ans2;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	*ans1 = ((-1)*b + quad) / 2*a;
	*ans2 = ((-1)*b - quad) / 2*a;

	printf("solved with formula 1\n")

}

///void ansFormula2(float a, float b, float c, float *ans1, float *ans2)
void ansFormula2(double a, double b, double c, double* ans1, double* ans2 )
{
	//if a =c, b = 2*a => subtracting similar number
	float quad;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	*ans1 = ((-2)*c) / (b+quad);
	*ans2 = ((-1)*b - quad) / 2*a;
}













