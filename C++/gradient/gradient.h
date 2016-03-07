#ifndef GRADIENT_H
#define GRADIENT_H

#include <iostream>
using namespace std;
#include "../codebarre/codebarre.h"


void gradient(CodeBarre u0, double epsilon, double alpha, double r,
	CodeBarre& u, double lambda);

double J(CodeBarre u, CodeBarre u0, double r, double lambda);

void gradientI(CodeBarre u0, double epsilon, double alpha, double r, 
	CodeBarre& u, double lambda, CodeBarre I, double mu);

double J(CodeBarre u, CodeBarre u0, double r, double lambda, CodeBarre I, 
	double mu);

#endif // GRADIENT_H
