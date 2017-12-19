#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn_3_2( int *n, double *x, double *fvec, int *iflag )
{	
	fvec[0] = x[1] + x[2]*10 +9;
	fvec[1] = sqrt(5)*(x[3]-x[0])-2*sqrt(5);
	fvec[2] = (x[2]-2*x[3])*(x[2]-2*x[3])-9;
	fvec[3] = sqrt(10)*(x[1]-x[0])*(x[1]-x[0])-2*sqrt(10);
}
// w    x     y     z
//x[0] x[1] x[2] x[3]

void program3_2(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS];	//need to initilize x0
	double fvec[SOLNUMS];	
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 *SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 *SOLNUMS + 13)) / 2;

	x[0] = 0.9; x[1] = -0.9; x[2] = 1.25; x[3] = -1.25;

	FILE *fp_w=NULL;

	fp_w = fopen("roots_found_3-2.txt", "w");
	if(fp_w == NULL) 
	{
		printf("%s file open error...\n", "roots_found_3-2.txt");
		return;
	}

	hybrd1_(fcn_3_2,&n,x,fvec,&tol,&info,wa,&lwa);

	fprintf(fp_w, "answer\nw = %.10lf, x = %.10lf, y = %.10lf, z = %.10lf\n\n", x[0],x[1],x[2],x[3]);
	fprintf(fp_w, "x+10y           = %.10lf\n", fvec[0]-9);
	fprintf(fp_w, "sqrt(5)(z-w)    = 2sqrt(5) = %.10lf\n", fvec[1]+2*sqrt(5));
	fprintf(fp_w, "(y-2z)^2        = %.10lf\n", fvec[2]+9);
	fprintf(fp_w, "sqrt(10)(x-w)^2 = 2sqrt(10) = %.10lf\n", fvec[3]+2*sqrt(10));

	printf("answer\nw = %.10lf, x = %.10lf, y = %.10lf, z = %.10lf\n\n", x[0], x[1], x[2], x[3]);
	printf("x+10y           = %.10lf\n", fvec[0]-9);
	printf("sqrt(5)(z-w)    = 2sqrt(5) = %.10lf \n", fvec[1]+2*sqrt(5));
	printf("(y-2z)^2        = %.10lf\n", fvec[2]+9);
	printf("sqrt(10)(x-w)^2 = 2sqrt(10) = %.10lf \n", fvec[3]+2*sqrt(10));

	if(fp_w != NULL) fclose(fp_w);
}










