#ifndef GRADIENT_H
#define GRADIENT_H

#endif // GRADIENT_H
#include <iostream>
using namespace std;
#include "../codebarre/codebarre.h"
#include <math.h>


void gradient(CodeBarre u0, double epsilon, double alpha, double r, CodeBarre& u,double lambda);
double J(CodeBarre u,CodeBarre u0,double r, double lambda);
void gradientI(CodeBarre u0, double epsilon, double alpha, double r, CodeBarre& u, double lambda, CodeBarre I, double mu);
double J(CodeBarre u, CodeBarre u0, double r, double lambda, CodeBarre I, double mu);
void gradientR(CodeBarre u0, double epsilon, double alpha, double rmin, double rmax, CodeBarre& ui, double lambda);
double trouveRayon(CodeBarre u0, double rmin, double rmax);
double pseudoNorme(double x, double y, double r);
