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

	// Step 0: pdf_table.txt 파일의 내용을 입력 받는다.
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
		
	// Step 1: 방정식 f(x) = F(x) - U = 0을 정의하는 f(x)를 만들기 위해 먼저 누적 분포 함수인 F(x)를 정의한다.
	// 누적 분포 함수의 정의는 4쪽을 참고하며, 구현은 합성 사다리꼴 공식을 사용하며 7쪽 3.2 부분 참고
		
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
		U = (double)rand() / RAND_MAX; // [0, 1] 사이에 존재하는 임의의 값 U
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
		// Step 2: 4주차 숙제로 작성했던 Bisection 방법을 사용해 f(x*) = 0으로 만들어 주는 x* 값을 구한다. (상수인 DELTA, Nmax, EPSILON은 본인 마음대로 조정 가능)
		// 이때 Bisection의 초기 구간은 x가 [0, 1]로 정규화 되었으므로 초기값을 0과 1로 둔다. 이때의 중점을 x*이라고 하자.
		// 그러나 우리가 구하려고 하는 x*은 이미 곡선을 설계할 때 사용된 sampling에 존재하는 x가 아닌 다른 유사 난수 x*를 찾는 것이다.
		// 따라서 x*에 해당하는 F(x*)값을 모르므로 f(x) = 0 방정식을 풀기 곤란하다.
		// Step 3: 따라서 F(x*) 값은 선형보간(linear interpolation)을 적용하여 근사적으로 F(x*)을 구한다.(선형보간 내용은 검색, 실습시간 설명 내용, 17쪽 5.2 부분 참고)
		
		//선형보간

		double result=0;
		result = (U-integY[startP])/(integY[endP]-integY[startP])*sampleX[startP] + (integY[endP]-U)/(integY[endP]-integY[startP])*sampleX[endP];
		
		fprintf(fp_w, "%lf", result);
		printf("%lf\n", result);
		// Step 4: 위 방법을 통해 구한 x*을 출력 포맷에 맞게 random_event_table.txt에 출력한다.(15쪽 참고)
	}

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}
