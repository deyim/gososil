#include "my_solver.h"

void program3_4(void)
{
	char direc[40] = "./Data/";
	char reading_file[30];
	char writing_file[30] = "solution_3-4_";
	char cur_writing_file_name[30];
	FILE *fp_r, *fp_w;
	int n, ia, *l;
	float *a, *b, *s, *x, *acopy, *bcopy;
	float errChk, errUp, errDown, errRow;
	int file_cnt = 0;
	char file_cnt_char[1];

	while (1) {
		strcpy(direc, "./Data/\0");
		printf("Enter input file name: (except directory)\n");
		scanf("%s", reading_file);
		printf("Enter output file name: \n");
		scanf("%s", cur_writing_file_name);
		strcat(direc, reading_file);


		fp_r = fopen(direc, "r");
		if (fp_r == NULL)
		{
			printf("%s file open error...\n", direc);
			return;
		}
		
		fscanf(fp_r, "%d", &n);
		ia = n;
		if (n > 32) {
			printf("ERROR: n is greater than 32");
			exit(-1);
		}
		
		a = (float*)malloc(sizeof(float)*n*n);
		acopy = (float*)malloc(sizeof(float)*n*n);
		b = (float*)malloc(sizeof(float)*n); // vectors
		bcopy = (float*)malloc(sizeof(float)*n);
		s = (float*)malloc(sizeof(float)*n);
		x = (float*)malloc(sizeof(float)*n);
		l = (int*)malloc(sizeof(int)*n);

		//for (int i = 0; i < n*n; i++) fscanf(fp_r, "%f\n", &a[i]);

		
		int row, col;
		int trow, tcol;
		for (int i = 0; i < n*n; i++) {
			row = i / n; col = i % n;
			trow = col; tcol = row;
			fscanf(fp_r, "%f\n", &a[trow*n+tcol]);
			acopy[row*n + col] = a[trow*n + tcol];
		}
		
		
		
		for (int i = 0; i < n; i++) {
			fscanf(fp_r, "%f\n", &b[i]);
			bcopy[i] = b[i];
		}
		gespp_(&n, a, &ia, l, s);
		solve_(&n, a, &ia, l, b, x);

		//writing
		fp_w = fopen(cur_writing_file_name, "w");
		if (fp_w == NULL)
		{
			printf("%s file open error...\n", writing_file);
			return;
		}

		fprintf(fp_w, "%d\n", n);
		printf("%d\n", n);
		for (int i = 0; i < n; i++) {
			fprintf(fp_w, "%.6f\n", x[i]);
			printf("%.6f\n", x[i]);
		}

		///error check
		errUp = 0; errRow = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				errRow += acopy[i*n+j] * x[j];
			}
			//printf("errRow: %f", errRow);
			errRow -= bcopy[i];
			errUp += (errRow*errRow);
		}
		errDown = 0;
		for (int i = 0; i < n; i++) {
			errDown += (bcopy[i] * bcopy[i]);
		}
		errChk = sqrt(errUp) / sqrt(errDown);
		fprintf(fp_w, "error: %.6f\n", errChk);
		printf("error: %.6f\n", errChk);
		fprintf(fp_w, "problem file: %s\n", direc);
		printf("problem file: %s\n", direc);
		fprintf(fp_w, "columnmajor\n");
		free(l);
		free(b);
		free(x);
		free(s);
	
		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}