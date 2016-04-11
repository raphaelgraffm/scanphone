#include "Neurone.h"
#include <iostream>
using namespace std;
#include <cassert>
#include<Imagine/Graphics.h>
using namespace Imagine;

double sigmaR(double x){
    double R = 0.;
    if (x < -50.)
        R = 0.;
    else if (x > 50.)
        R = 1.;
    else
        R = exp(x)/(1+exp(x));
    return R;
}


Perceptron::Perceptron(){
    n = 0;
}

Perceptron::Perceptron(int n0){
    n = n0;
    w = new double[n0];
    //Initialisation au hasard de w
    for (int i=0; i<n0; i++) {
        w[i] = 0;
    }
    theta = 0;
}

//Initialiser w a partir d'un tableau
void Perceptron::set(double ww[], double ttheta){
    for (int i=0; i<n; i++)
        w[i] = ww[i];
    theta = ttheta;
}

int Perceptron::size() const{
    return n;
}

double Perceptron::seuil() {
    return theta;
}

double Perceptron::operator[](int i){
    return w[i];
}

double Perceptron::Heaviside(double I[]){
    int S = 0.;
    for (int i=0; i<n; i++)
        S += w[i]*I[i];
    if (S>theta)
        return 1;
    else
        return 0;
}

double Perceptron::Adaline(double I[]){
    double S = 0.;
    for (int i=0; i<n; i++)
        S += w[i]*I[i];
    return (S-theta);
}



//ECHANTILLON

Echantillon::Echantillon(int p0, int n0, int k0){
    p = p0;
    n = n0;
    k = k0;

    el = new double*[p0];
    for (int i=0; i<p0; i++)
        el[i] = new double[n0];

    v = new double*[p0];
    for (int i=0; i<p0; i++)
        v[i] = new double[k0];
}

int Echantillon::size() const{
    return p;
}

void Echantillon::set(int i, double ele[], double retour[]){
    for (int j=0; j<n; j++)
        el[i][j] = ele[j];
    for (int j=0; j<k; j++)
        v[i][j] = retour[j];
}

void Echantillon::setEl(int i, int j, double d){
    el[i][j] = d;
}

void Echantillon::setT(int i, int j, double d){
    v[i][j] = d;
}

double* Echantillon::getEl(int i){
    return el[i];
}

double* Echantillon::tabT(int i){
    return v[i];
}

double Echantillon::T(int i, int j){
    return v[i][j];
}


Echantillon::~Echantillon(){
    delete[] el;
    delete[] v;
}


double Echantillon::O(int i, Perceptron P){
    return P.Adaline(el[i]);
}

//RESEAU

Reseau::Reseau(int q0, int nL0){
    q = q0;
    nLmax = nL0;
    p = new Perceptron*[q];
    z = new double*[q+1];
    derE = new double*[q];
    for (int L=0; L<q; L++){
        p[L] = new Perceptron[nL0];
        for (int i=0; i<nL0; i++){
            p[L][i].n = nL0;
            p[L][i].w = new double[nL0];
            p[L][i].theta = 0.;
        }
        z[L] = new double[nL0];
        derE[L] = new double[nL0];
    }
    z[q] = new double[nL0];

    // aléatoire
    for (int L=0; L<q; L++)
        for (int i=0; i<nLmax; i++) {
            for (int j=0; j<nLmax; j++){
                p[L][i].w[j] = 0;
                p[L][i].w[j] = double(rand()%1000000)/1000000;
            }
            p[L][i].theta = double(rand()%1000000)/1000000;
        }
}

Reseau::~Reseau(){
    //delete[] p;
    //delete[] z;
    //delete[] derE;
}

void Reseau::calculeZ(double I[]){
    //cout << "Toast ?" << endl;
    for (int i=0; i<nLmax; i++)
        z[0][i] = I[i];
    for (int L=1; L<=q; L++)
        for (int i=0; i<nLmax; i++){
            z[L][i] = sigmaR(p[L-1][i].Adaline(z[L-1]));
            //cout << "z[" << L <<"][" << i << "] = "  << z[L][i] << endl;
        }
    //cout << "La base a réussi: " << z[q][0] << endl;
}



double* Reseau::retourSortie(){
    return z[q];
}

void Reseau::majP(double T[], double eps){
    //Calcul des dE/dyj

    for (int j=0; j<nLmax; j++)
        derE[q-1][j] = z[q][j]*(1-z[q][j])*(z[q][j]-T[j]);

    for (int L=q-2; L>=0; L--)
        for (int j=0; j<nLmax; j++){
            derE[L][j] = 0.;
            for (int k=0; k<nLmax; k++)
                derE[L][j] += p[L+1][k].w[j]*derE[L+1][k];
            derE[L][j] *= z[L+1][j]*(1-z[L+1][j]);
            //cout << double(derE[L][j]) << endl;
    }

    //Mise à jour des perceptrons
    for (int L=q-1; L>=0; L--)
        for (int i=0; i<nLmax; i++) {
            for (int j=0; j<nLmax; j++) {
                p[L][i].w[j] -= eps*z[L][i]*derE[L][j];
            }
            p[L][i].theta -= eps*derE[L][i]*(-1);
        }

    //cout << z[q][0] << endl;
    //cout << derE[0][0] << endl;
}

void Reseau::apprend(Echantillon E, double eps){
    for (int i=0; i<E.size(); i++){
        calculeZ(E.getEl(i));
        //cout << E.getEl(i)[0] << endl;
        majP(E.tabT(i),eps);
    }
}

void Reseau::affiche() {
    for (int L=0; L<q; L++) {
        cout << "--- Couche " << L << endl;
        for (int i=0; i<nLmax; i++) {
            for (int j=0; j<nLmax; j++) {
                cout << p[L][i].w[j] << " | ";
            }
            cout << "th : " << p[L][i].theta << endl;
        }
    }
}
