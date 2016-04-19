#include "Neurone.h"

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

Perceptron::~Perceptron() {
    delete[] w;
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
    p = p0; // nombre d'exemples
    n = n0; // entrées
    k = k0; // sorties

    el = new double*[p0];
    for (int i=0; i<p0; i++)
        el[i] = new double[n0];

    v = new double*[p0];
    for (int i=0; i<p0; i++)
        v[i] = new double[k0];
}


Echantillon::~Echantillon(){
    for (int i=0; i<p; i++) {
        delete[] el[i];
        delete[] v[i];
    }
    delete[] el;
    delete[] v;
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


double Echantillon::O(int i, Perceptron P){
    return P.Adaline(el[i]);
}

//RESEAU
Reseau::Reseau() {
    q=0;
}


void Reseau::set(int q0, int nL[]){
    // en supposant nL de taille au moins q+1 (en comptant l'entrée)
    q = q0; // nombre de couches

    nLmax = new int[q+1];
    for(int i=0;i<=q;i++) {
        nLmax[i] = nL[i];
    }

    p = new Perceptron*[q];
    z = new double*[q+1]; // +1 en comptant l'entrée
    derE = new double*[q];

    for (int L=0; L<q; L++){
        p[L] = new Perceptron[nLmax[L+1]]; // attention au décalage de n
        for (int i=0; i<nLmax[L+1]; i++){
            p[L][i].n = nLmax[L];
            p[L][i].w = new double[nLmax[L]];
            p[L][i].theta = 0.;
        }
        z[L] = new double[nLmax[L]];
        derE[L] = new double[nLmax[L+1]]; // +1 à vérifier, cf algo
    }
    z[q] = new double[nLmax[q]];

    // aléatoire
    for (int L=0; L<q; L++)
        for (int i=0; i<nLmax[L+1]; i++) {
            for (int j=0; j<nLmax[L]; j++){
                //p[L][i].w[j] = 0.0001;
                p[L][i].w[j] = double(rand()%100000)/1000000000;
            }
            p[L][i].theta = double(rand()%100000)/1000000000;
        }
}

Reseau::~Reseau(){
    delete[] nLmax;

    /*for(int L=0;L<q;L++) {
        delete[] p[L];
        delete[] z[L];
        delete[] derE[L];
    }
    delete[] z[q];
    delete[] p;
    delete[] z;
    delete[] derE;*/
}

void Reseau::calculeZ(double I[]){
    for (int i=0; i<nLmax[0]; i++) {
        z[0][i] = I[i];
    }
    for (int L=1; L<=q; L++)
        for (int i=0; i<nLmax[L]; i++){
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

    for (int j=0; j<nLmax[q]; j++)
        derE[q-1][j] = z[q][j]*(1-z[q][j])*(z[q][j]-T[j]);

    for (int L=q-2; L>=0; L--)
        for (int j=0; j<nLmax[L+1]; j++){
            derE[L][j] = 0.;
            for (int k=0; k<nLmax[L+2]; k++)
                derE[L][j] += p[L+1][k].w[j]*derE[L+1][k];
            derE[L][j] *= z[L+1][j]*(1-z[L+1][j]);
            //cout << double(derE[L][j]) << endl;
    }

    //Mise à jour des perceptrons
    for (int L=q-1; L>=0; L--)
        for (int i=0; i<nLmax[L+1]; i++) {
            for (int j=0; j<nLmax[L]; j++) {
                p[L][i].w[j] -= eps*derE[L][i]*z[L][j];
            }
            p[L][i].theta -= eps*derE[L][i]*(-1);
        }

    //cout << z[q][0] << endl;
    //cout << derE[0][0] << endl;
}

void Reseau::apprend(Echantillon E, double eps){
    for (int i=0; i<E.size(); i++){
        calculeZ(E.getEl(i));
        //cout << "Résultat réseau : " << z[q][0] << " or il faut " << (E.tabT(i))[0];
        majP(E.tabT(i),eps);
        calculeZ(E.getEl(i));
        //cout << ", après correction : " << z[q][0] << endl;
    }
}

void Reseau::affiche() {
    for (int L=0; L<q; L++) {
        cout << "--- Couche " << L << endl;
        for (int i=0; i<nLmax[L+1]; i++) {
            for (int j=0; j<nLmax[L]; j++) {
                cout << p[L][i].w[j] << " | ";
            }
            cout << "th : " << p[L][i].theta << endl;
        }
    }
}


void Reseau::ecritureFichier(ofstream& fileNet) {
    // Fonction affiche modifiée pour écrire dans un fichier

    // Écriture des paramètres du réseau
    for (int L=0; L<q; L++) {
        fileNet << nLmax[L] << " ";
    }
    fileNet << nLmax[q] << endl;

    // Écriture de tous les poids des perceptron
    for (int L=0; L<q; L++) {
        for (int i=0; i<nLmax[L+1]; i++) {
            for (int j=0; j<nLmax[L]; j++) {
                fileNet << p[L][i].w[j] << " ";
            }
            fileNet << p[L][i].theta << endl;
        }
    }
}
