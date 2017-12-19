#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

// global variables
const double DELTA = 0.01;
const int Nmax = 100;
const double EPSILON = 0.01;

void program2_2()
{
	FILE *fp_r, *fp_w;	
	int n_r;
	double U;
	double *sampleX, *sampleY, *integY;
	double tmpX, tmpY;
	int cnt = 0;
	double integSum=0, h =0;
	int startP, endP, x1;

//	srand(time(NULL));

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	scanf("%d", &n_r);

	// Step 0: pdf_table.txt ������ ������ �Է� �޴´�.
	while(!feof(fp_r)){
		fscanf(fp_r, "%lf %lf\n", &tmpX, &tmpY);
		cnt++;
	}
	fclose(fp_r);
	h = (double)1/cnt;
	sampleX = (double*)malloc(sizeof(double)*cnt);
	sampleY = (double*)malloc(sizeof(double)*cnt);
	integY = (double*)malloc(sizeof(double)*cnt);

	fp_r = fopen("pdf_table.txt", "r");
	for(int i = 0 ; i < cnt ; i++){
		fscanf(fp_r, "%lf %lf", &sampleX[i], &sampleY[i]);
		
	// Step 1: ������ f(x) = F(x) - U = 0�� �����ϴ� f(x)�� ����� ���� ���� ���� ���� �Լ��� F(x)�� �����Ѵ�.
	// ���� ���� �Լ��� ���Ǵ� 4���� �����ϸ�, ������ �ռ� ��ٸ��� ������ ����ϸ� 7�� 3.2 �κ� ����
		
	} 
	integSum=0;
	integY[0] = 0;
	for(int i = 1 ; i < cnt ; i++){
		integSum = (sampleY[i]+sampleY[i-1]) /2;
		integY[i] = integY[i-1] + integSum;
	}

	int i = 0, irand;
	unsigned int iseed = (unsigned int) time(NULL);
	srand(iseed);

	while (n_r--) {
		U = (double)rand() / RAND_MAX; // [0, 1] ���̿� �����ϴ� ������ �� U
		startP = 0; endP = cnt-1;

		x1 = (startP + endP) /2;
		for(int n=0;;n++){

			if(fabs(integY[x1]-U) < DELTA)break;
			else if( n >= Nmax) break;
			else if(fabs(integY[endP]-integY[startP]) < EPSILON) break;
			
			if((integY[x1]-U)*(integY[startP]-U) > 0) startP = x1;
			else if((integY[x1]-U) * (integY[endP]- U)> 0) endP = x1;
			else break;

			x1 = (startP+endP)/2;
		}
		// Step 2: 4���� ������ �ۼ��ߴ� Bisection ����� ����� f(x*) = 0���� ����� �ִ� x* ���� ���Ѵ�. (����� DELTA, Nmax, EPSILON�� ���� ������� ���� ����)
		// �̶� Bisection�� �ʱ� ������ x�� [0, 1]�� ����ȭ �Ǿ����Ƿ� �ʱⰪ�� 0�� 1�� �д�. �̶��� ������ x*�̶�� ����.
		// �׷��� �츮�� ���Ϸ��� �ϴ� x*�� �̹� ��� ������ �� ���� sampling�� �����ϴ� x�� �ƴ� �ٸ� ���� ���� x*�� ã�� ���̴�.
		// ���� x*�� �ش��ϴ� F(x*)���� �𸣹Ƿ� f(x) = 0 �������� Ǯ�� ����ϴ�.
		// Step 3: ���� F(x*) ���� ��������(linear interpolation)�� �����Ͽ� �ٻ������� F(x*)�� ���Ѵ�.(�������� ������ �˻�, �ǽ��ð� ���� ����, 17�� 5.2 �κ� ����)
		
		//��������

		double result=0;
		result = (U-integY[startP])/(integY[endP]-integY[startP])*sampleX[startP] + (integY[endP]-U)/(integY[endP]-integY[startP])*sampleX[endP];
		
		fprintf(fp_w, "%lf", result);
		printf("%lf\n", result);
		// Step 4: �� ����� ���� ���� x*�� ��� ���˿� �°� random_event_table.txt�� ����Ѵ�.(15�� ����)
	}

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}
