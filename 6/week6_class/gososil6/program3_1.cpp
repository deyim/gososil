
#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001
double p[4][3], t[4], tr[4];
double C, b;



void fcn3_1_1( int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag )
{
	// origin function F(x)
	if( *iflag == 1 ){
		fvec[0] = (x[0] - p[0][0])*(x[0] - p[0][0]) + (x[1] - p[0][1])*(x[1] - p[0][1]) + (x[2] - p[0][2])*(x[2] - p[0][2]) - (C*(tr[0] + x[3] - t[0]))*(C*(tr[0] + x[3] - t[0]));
		fvec[1] = (x[0] - p[1][0])*(x[0] - p[1][0]) + (x[1] - p[1][1])*(x[1] - p[1][1]) + (x[2] - p[1][2])*(x[2] - p[1][2]) - (C*(tr[1] + x[3] - t[1]))*(C*(tr[1] + x[3] - t[1]));
		fvec[2] = (x[0] - p[2][0])*(x[0] - p[2][0]) + (x[1] - p[2][1])*(x[1] - p[2][1]) + (x[2] - p[2][2])*(x[2] - p[2][2]) - (C*(tr[2] + x[3] - t[2]))*(C*(tr[2] + x[3] - t[2]));
		fvec[3] = (x[0] - p[3][0])*(x[0] - p[3][0]) + (x[1] - p[3][1])*(x[1] - p[3][1]) + (x[2] - p[3][2])*(x[2] - p[3][2]) - (C*(tr[3] + x[3] - t[3]))*(C*(tr[3] + x[3] - t[3]));
	}
	// Jacobi matrix J(x)
	else if( *iflag == 2 ){
		fjac[0] = 2.0*(x[0] - p[0][0]);		fjac[4] = 2.0*(x[1] - p[0][1]);		fjac[8] = 2.0*(x[2] - p[0][2]);		fjac[12] = (-2.0)*C*C*tr[0] - (2.0*C*C*x[3]) + 2.0*C*C*t[0];
		fjac[1] = 2.0*(x[0] - p[1][0]);		fjac[5] = 2.0*(x[1] - p[1][1]);		fjac[9] = 2.0*(x[2] - p[1][2]);		fjac[13] = (-2.0)*C*C*tr[1] - (2.0*C*C*x[3]) + 2.0*C*C*t[1];
		fjac[2] = 2.0*(x[0] - p[2][0]);		fjac[6] = 2.0*(x[1] - p[2][1]);		fjac[10] = 2.0*(x[2] - p[2][2]);		fjac[14] = (-2.0)*C*C*tr[2] - (2.0*C*C*x[3]) + 2.0*C*C*t[2];
		fjac[3] = 2.0*(x[0] - p[3][0]);		fjac[7] = 2.0*(x[1] - p[3][1]);		fjac[11] = 2.0*(x[2] - p[3][2]);		fjac[15] = (-2.0)*C*C*tr[3] - (2.0*C*C*x[3]) + 2.0*C*C*t[3];
	}
}

void fcn_3_1_2( int *n, double *x, double *fvec, int *iflag )
{	
	fvec[0] = (x[0] - p[0][0])*(x[0] - p[0][0]) + (x[1] - p[0][1])*(x[1] - p[0][1]) + (x[2] - p[0][2])*(x[2] - p[0][2]) - (C*(tr[0] + x[3] - t[0]))*(C*(tr[0] + x[3] - t[0]));
	fvec[1] = (x[0] - p[1][0])*(x[0] - p[1][0]) + (x[1] - p[1][1])*(x[1] - p[1][1]) + (x[2] - p[1][2])*(x[2] - p[1][2]) - (C*(tr[1] + x[3] - t[1]))*(C*(tr[1] + x[3] - t[1]));
	fvec[2] = (x[0] - p[2][0])*(x[0] - p[2][0]) + (x[1] - p[2][1])*(x[1] - p[2][1]) + (x[2] - p[2][2])*(x[2] - p[2][2]) - (C*(tr[2] + x[3] - t[2]))*(C*(tr[2] + x[3] - t[2]));
	fvec[3] = (x[0] - p[3][0])*(x[0] - p[3][0]) + (x[1] - p[3][1])*(x[1] - p[3][1]) + (x[2] - p[3][2])*(x[2] - p[3][2]) - (C*(tr[3] + x[3] - t[3]))*(C*(tr[3] + x[3] - t[3]));
}

void program3_1(void)
{
	//TODO
	int n = SOLNUMS;
	double x[SOLNUMS], x2[SOLNUMS];
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double *wa;
	int lwa;


	wa = (double*)malloc(sizeof(double)*(SOLNUMS * (SOLNUMS+13)) / 2);
	lwa = (SOLNUMS * (SOLNUMS+13)) / 2;

	char reading_file_name[30] = "GPS_signal_";
	char writing_file_name[30] = "GPS_position_3-1_";
	char writing_file_name2[30] = "GPS_position_3-2_";
	char cur_reading_file_name[30];
	char cur_writing_file_name[30];
	char file_cnt_char[3];
	int file_cnt=0;

	FILE *fp_r=NULL, *fp_w=NULL;

	for(file_cnt = 0 ; file_cnt < 3; file_cnt++){
		//modify file name
		sprintf(file_cnt_char, "%d", file_cnt);
		strcpy(cur_reading_file_name, reading_file_name);
		strcat(cur_reading_file_name, file_cnt_char);
		strcat(cur_reading_file_name, ".txt");
		strcpy(cur_writing_file_name, writing_file_name);
		strcat(cur_writing_file_name, file_cnt_char);
		strcat(cur_writing_file_name, ".txt");

		//file open and init
		FILE *fp_r = fopen(cur_reading_file_name, "r");
		if(fp_r== NULL) 
		{
			printf("%s file open error...\n", cur_reading_file_name);
			return;
		}	

		FILE *fp_w = fopen(cur_writing_file_name, "w");
		if(fp_w == NULL) 
		{
			printf("%s file open error...\n", cur_writing_file_name);
			return;
		}	

		for(int i = 0 ; i < 4 ; i++){
			scanf("%lf", &x[i]);
			x2[i] = x[i];
		}

		//get elements from file
		fscanf(fp_r, "%lf %lf", &C, &b);
		for(int i = 0 ; i < 4 ; i++){
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &p[i][0], &p[i][1], &p[i][2], &t[i], &tr[i]);
		}

		//hybrj function
		hybrj1_(fcn3_1_1,	&n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);	

		fprintf(fp_w, "-----------------hybrj1-----------------------\n");
		fprintf(fp_w, "answer: %.10lf, %.10lf, %.10lf, %.10lf\n", x[0],x[1],x[2],x[3]);
		fprintf(fp_w, "f1(x1,x2,x3,x4) = %.10lf\n", fvec[0]);
		fprintf(fp_w, "f2(x1,x2,x3,x4) = %.10lf\n", fvec[1]);
		fprintf(fp_w, "f3(x1,x2,x3,x4) = %.10lf\n", fvec[2]);
		fprintf(fp_w, "f3(x1,x2,x3,x4) = %.10lf\n\n", fvec[3]);

		printf("-----------------hybrj1-----------------------\n");
		printf("answer: %.10lf, %.10lf, %.10lf, %.10lf\n", x[0], x[1], x[2], x[3]);
		printf("f1(x1,x2,x3,x4) = %.10lf\n", fvec[0]);
		printf("f2(x1,x2,x3,x4) = %.10lf\n", fvec[1]);
		printf("f3(x1,x2,x3,x4) = %.10lf\n", fvec[2]);
		printf("f3(x1,x2,x3,x4) = %.10lf\n\n", fvec[3]);
		if(fp_w != NULL) fclose(fp_w);


		///hybrd function
		strcpy(cur_writing_file_name, writing_file_name2);
		strcat(cur_writing_file_name, file_cnt_char);
		strcat(cur_writing_file_name, ".txt");

		fp_w = fopen(cur_writing_file_name, "w");
		if(fp_w == NULL) 
		{
			printf("%s file open error...\n", cur_writing_file_name);
			return;
		}
		free(wa);
		wa = (double*)malloc(sizeof(double)*(SOLNUMS * (3 * SOLNUMS + 13)) / 2);
		lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;
		for (int i = 0; i < SOLNUMS; i++) fvec[i] = 0;


		hybrd1_(fcn_3_1_2,&n,x2,fvec,&tol,&info,wa,&lwa);

		fprintf(fp_w, "-----------------hybrd1-----------------------\n");
		fprintf(fp_w, "answer: %.10lf, %.10lf, %.10lf, %.10lf\n", x2[0], x2[1], x2[2], x2[3]);
		fprintf(fp_w, "f1(x1,x2,x3,x4) = %.10lf\n", fvec[0]);
		fprintf(fp_w, "f2(x1,x2,x3,x4) = %.10lf\n", fvec[1]);
		fprintf(fp_w, "f3(x1,x2,x3,x4) = %.10lf\n", fvec[2]);
		fprintf(fp_w, "f3(x1,x2,x3,x4) = %.10lf\n\n", fvec[3]);

		printf("-----------------hybrd1-----------------------\n");
		printf("answer: %.10lf, %.10lf, %.10lf, %.10lf\n", x2[0], x2[1], x2[2], x2[3]);
		printf("f1(x1,x2,x3,x4) = %.10lf\n", fvec[0]);
		printf("f2(x1,x2,x3,x4) = %.10lf\n", fvec[1]);
		printf("f3(x1,x2,x3,x4) = %.10lf\n", fvec[2]);
		printf("f3(x1,x2,x3,x4) = %.10lf\n\n", fvec[3]);

		if(fp_w != NULL) 
			fclose(fp_w);

	}
	if(fp_r != NULL) fclose(fp_w);

}