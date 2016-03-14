#include "gradient.h"
#include <iostream>
using namespace std;


double J(CodeBarre u,CodeBarre u0,double r, double lambda){
    CodeBarre u1 = u.flou(r);
    CodeBarre u2=u1-u0;
    // pour éviter dépassement du double, *0.0001
    return 0.0001*(0.5*(u2.normeL2())+lambda*((u.grad()).normeL1()));
}

double J(CodeBarre u, CodeBarre u0, double r, double lambda, CodeBarre I, double mu){
    return(0.5*(u.flou(r)-u0).normeL2()+lambda*((u.grad()).normeL1())+0.5*mu*((I.grad()).normeL2()));
}


void gradient(CodeBarre u0, double epsilon, double alpha, double r, CodeBarre& u, double lambda){
    int iteration=0;
    while(J(u,u0,r,lambda)>epsilon && iteration<200){
        cout<<J(u,u0,r,lambda)<<endl;
        CodeBarre du;
        du = u.grad();
        double a = 1/(sqrt(du.normeL2())+1);
        u = u - ( ((u.flou(r)-u0).flou(r)-du.div()*a*lambda)*alpha );
        u.normaliser();
        iteration++;
    }
}

void gradientI(CodeBarre u0, double epsilon, double alpha, double r, CodeBarre& u, double lambda, CodeBarre I, double mu){
    while(J(u,u0,r,lambda,I,mu)>epsilon){
        I=I-(((u.flou(r))*(I*(u.flou(r))-u0)-(((I.grad()).div())*mu))*alpha);
        u=u-((I*((I*(u.flou(r))-u0).flou(r))-((u.grad()*(1/((u.grad()).normeL1()))).div())*lambda)*alpha);
    }
}

void gradientR(CodeBarre u0, double epsilon, double alpha, double rmin, double rmax, CodeBarre& ui, double lambda){
    double r;
    r=trouveRayon(u0,rmin,rmax);
    cout<<"Rayon"<<" "<<"="<<" "<<r<<endl;
    gradient(u0,epsilon,alpha,r,ui,lambda);
}

double trouveRayon(CodeBarre u0, double rmin, double rmax){
    while((rmax-rmin)>0.01){
        double S1=0;
        double S2=0;
        for(int i=0;i<largeurImage-1;i++){
            S1 += pseudoNorme(u0(i),u0(i+1),rmin);
            S2 += pseudoNorme(u0(i),u0(i+1),rmax);
        }
        if(S1<S2){
            rmax=(rmax+rmin)/2;
        }else{
            rmin=(rmax+rmin)/2;
        }
    }
    return((rmax+rmin)/2);
}

double pseudoNorme(double x, double y, double r){
    int n = int(r);
    int a = r-n;
    double m = (255*(2*n+3));
    double s;
    for(int p1=0;p1<3;p1++){
        for(int p2=0;p2<3;p2++){
            s = abs(((2*r+1)*(y-x)-255*((1-a)*(p1-1)+a*(p2-1))));
            if(s<m){
                m=s;
            }
        }
    }
    return m;
}
