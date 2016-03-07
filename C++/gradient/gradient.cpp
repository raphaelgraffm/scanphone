#include "gradient.h"


double J(CodeBarre u,CodeBarre u0,double r, double lambda)
{
    return(0.5*(u.flou(r)-u0).normeL2()+lambda*((u.grad()).normeL1()));
}

double J(CodeBarre u, CodeBarre u0, double r, double lambda, CodeBarre I, 
	double mu)
{
    return(0.5*(u.flou(r)-u0).normeL2()+lambda*((u.grad()).normeL1())+
    	0.5*mu*((I.grad()).normeL2()));
}


void gradient(CodeBarre u0, double epsilon, double alpha, double r, 
	CodeBarre& u, double lambda)
{ 
    while(J(u,u0,r,lambda) > epsilon)
    {
        u = u-((u.flou(r)-u0).flou(r)-((u.grad()*
        	(1/((u.grad()).normeL1()))).div())*lambda)*alpha;
    }
}

void gradientI(CodeBarre u0, double epsilon, double alpha, double r, 
	CodeBarre& u, double lambda, CodeBarre I, double mu)
{
    while( J(u,u0,r,lambda,I,mu) > epsilon )
    { 
        I = I-((u.flou(r))*(I*(u.flou(r))-u0)-((I.grad()).div())*mu)*alpha;
        u = u-(I*((I*(u.flou(r))-u0).flou(r))-((u.grad()*
        	(1/((u.grad()).normeL1()))).div())*lambda)*alpha;
    }
}
