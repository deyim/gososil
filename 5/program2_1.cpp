#include "my_solver.h"

void program2_1()
{
	FILE *fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;

	int cnt;
	double span, spanFor1=0;
	double *sampleX, *sampleY, *converedX, *converedY;
	double integCur = 0, integSum = 0;
	double check=0;
	double length= 0 ;
	double startPoint, endPoint;

	fp_r = fopen("sampling_table.txt", "r");		
	if(fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	// Step 0: sampling_table.txt 파일의 내용을 입력 받는다.
	fscanf(fp_r, "%d %lf", &cnt, &span);
	sampleX = (double*)malloc(sizeof(double)*cnt);
	sampleY = (double*)malloc(sizeof(double)*cnt);
	converedX = (double*)malloc(sizeof(double)*cnt);
	converedY = (double*)malloc(sizeof(double)*cnt);
	for(int i = 0 ; i < cnt; i++){
		fscanf(fp_r, "%lf %lf\n", &sampleX[i], &sampleY[i]);
		if(i != 0 && i != cnt-1)
			integSum += (2*sampleY[i]);
	}
	integSum += (sampleY[0] + sampleY[cnt-1]);
	//spanFor1 = span / (sampleX[cnt-1] - sampleX[0]);
	integSum = integSum * span / 2;
	// Step 1: x의 구간을 [0, 1] 사이로 정규화한다.
	// 이때 정규화 하면서 x의 간격이 바뀌였기 때문에 나중에 적분시 사다리꼴의 높이가 바뀐 것을 주의해야 한다!!

	// Step 2: 곡선 함수를 확률밀도함수(pdf)로 만들기 위해 정규화한 x의 전체 구간에서 y값을 수치적으로 적분했을 때 1이 나오게 y값을 변환한다. 
	// (이제 이 곡선 함수는 확률밀도 함수의 조건을 만족했으므로 y를 p(x)라고 하겠다.)
	// y -> p(x)로 바꾸는 식은 강의자료 11쪽 참고 (분모의 x의 전체구간을 적분하는 수식은 합성 사다리꼴 공식을 사용하며 7쪽 3.2 부분 참고)

	//converedY[0] = 

	
	// Step 3: 위에서 구한 x와 p(x) 값을 sampling_table.txt와 같은 포맷으로 pdf_table.txt에 저장한다.

	length = (sampleX[cnt-1] - sampleX[0]) / cnt;
	for(int i = 0 ; i < cnt ; i++) {
		converedX[i] = (sampleX[i]-sampleX[0]) / (sampleX[cnt-1]-sampleX[0]);
		converedY[i] = sampleY[i] / (integSum) * length;
		fprintf(fp_w, "%.6lf %.6lf\n", converedX[i], converedY[i]);
		//check += converedY[i];
	}

	



	// Step 4: 위에서 구한 p(x)에 대해서 x의 전체구간 적분을 했을 때 1에 근사하게 나오는지 확인한다.
	// Console 창에 강의자료 12쪽 포맷을 참고해서 출력한다.

	//integSum += (converedY[0] + converedY[cnt-1]);
	//integSum = integSum * spanFor1 /2 ;
	startPoint = 0.0; endPoint = 1.0; check = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(converedX[i] > endPoint || converedX[i] < startPoint) continue;
		check += converedY[i];
	}
	printf("*** Integrating the pdf from 0.0 to 1.0 = %.6lf\n", check);

	startPoint = 0.0; endPoint = 0.25; check = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(converedX[i] > endPoint || converedX[i] < startPoint) continue;
		check += converedY[i];
	}
	printf("*** Integrating the pdf from 0.0 to 0.25 = %.6lf\n", check);

	startPoint = 0.25; endPoint = 0.5; check = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(converedX[i] > endPoint || converedX[i] < startPoint) continue;
		check += converedY[i];
	}
	printf("*** Integrating the pdf from 0.25 to 0.5 = %.6lf\n", check);

	startPoint = 0.5; endPoint = 0.75; check = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(converedX[i] > endPoint || converedX[i] < startPoint) continue;
		check += converedY[i];
	}
	printf("*** Integrating the pdf from 0.5 to 1.75 = %.6lf\n", check);

	startPoint = 0.75; endPoint = 1.0; check = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(converedX[i] > endPoint || converedX[i] < startPoint) continue;
		check += converedY[i];
	}
	printf("*** Integrating the pdf from 0.75 to 1.0 = %.6lf\n", check);

	

	if(fp_r != NULL) fclose(fp_r);
	if(fp_w != NULL) fclose(fp_w);
}