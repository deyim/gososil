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
	scanf("%f%f%f", &a, &b, &c);

	if(a ==0){
		printf("a cannot be zero re enter\n");
		scanf("%f%f%f", &a, &b, &c);
	} 

	ansFormula1(a,b,c, &ans1, &ans2);

	printf("solved with formula 1\n");
	printf("ans1: %f, ans2: %f\n", ans1, ans2);

	//ansFormula2(a,b,c, &ans1, &ans2);
 }

void ansFormula1(float a, float b, float c, float *ans1, float *ans2)
{
	float quad;
	quad = b*b - 4*a*c;
	quad = sqrt(quad);
	*ans1 = ((-1)*b - quad) / 2*a;
	printf("ans1: %f", *ans1);
	*ans2 = (b - quad) / 2*a;

}

void ansFormula2(float a, float b, float c, float *ans1, float *ans2)
{
	;
}

