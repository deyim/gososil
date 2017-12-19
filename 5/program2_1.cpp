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

	// Step 0: sampling_table.txt ������ ������ �Է� �޴´�.
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
	// Step 1: x�� ������ [0, 1] ���̷� ����ȭ�Ѵ�.
	// �̶� ����ȭ �ϸ鼭 x�� ������ �ٲ�� ������ ���߿� ���н� ��ٸ����� ���̰� �ٲ� ���� �����ؾ� �Ѵ�!!

	// Step 2: � �Լ��� Ȯ���е��Լ�(pdf)�� ����� ���� ����ȭ�� x�� ��ü �������� y���� ��ġ������ �������� �� 1�� ������ y���� ��ȯ�Ѵ�. 
	// (���� �� � �Լ��� Ȯ���е� �Լ��� ������ ���������Ƿ� y�� p(x)��� �ϰڴ�.)
	// y -> p(x)�� �ٲٴ� ���� �����ڷ� 11�� ���� (�и��� x�� ��ü������ �����ϴ� ������ �ռ� ��ٸ��� ������ ����ϸ� 7�� 3.2 �κ� ����)

	//converedY[0] = 

	
	// Step 3: ������ ���� x�� p(x) ���� sampling_table.txt�� ���� �������� pdf_table.txt�� �����Ѵ�.

	length = (sampleX[cnt-1] - sampleX[0]) / cnt;
	for(int i = 0 ; i < cnt ; i++) {
		converedX[i] = (sampleX[i]-sampleX[0]) / (sampleX[cnt-1]-sampleX[0]);
		converedY[i] = sampleY[i] / (integSum) * length;
		fprintf(fp_w, "%.6lf %.6lf\n", converedX[i], converedY[i]);
		//check += converedY[i];
	}

	



	// Step 4: ������ ���� p(x)�� ���ؼ� x�� ��ü���� ������ ���� �� 1�� �ٻ��ϰ� �������� Ȯ���Ѵ�.
	// Console â�� �����ڷ� 12�� ������ �����ؼ� ����Ѵ�.

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