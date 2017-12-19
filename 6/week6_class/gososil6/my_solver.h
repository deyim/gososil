#ifndef _MY_SOLVER
#define _MY_SOLVER
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


const double EPSILON = 0.0000001;

extern "C"
{
	void gespp_(int *, float*, int *, int *, float *);
	void solve_(int *, float*, int *, int *, float *, float *);
	int rpoly_(double *, int *, double *, double *, long int *);
	int hybrj1_(void fcn(int *, double *, double *, double *, int *, int *), int *, double *, double *, double *, int *, double *, int *, double *, int *);
	int hybrd1_(void fcn(int *, double *, double *, int *), int *, double *, double *, double *, int *, double *, int *);
};

#endif