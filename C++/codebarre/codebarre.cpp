#include "codebarre.h"
#include <iostream>
using namespace std;
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;
#include <math.h>


// ----- constructeurs + accesseur

CodeBarre::CodeBarre() {
    // constructeur vide
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=0;
    }
}

double& CodeBarre::operator()(int i) {
    // permet de gèrer les conditions au bord
    // pour les fonctions qui en ont besoin
    if (i<0) {
        return data[0];
    }
    else if (i>=n) {
        return data[n-1];
    }
    else {
        return data[i];
    }
}

CodeBarre::CodeBarre(int valInitiale) {
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=valInitiale;
    }
}

CodeBarre::CodeBarre(int d[largeurImage]) {
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=d[i];
    }
}

CodeBarre::CodeBarre(const CodeBarre& u) {
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=u.data[i];
    }
}

// constructeur à partir d'un chemin, faire srcPath dans l'appel
CodeBarre::CodeBarre(const char* chemin) : n(largeurImage)
{
    // importe l'image
    Image<byte> img;
    if ( !load(img,chemin) )
    {
        std::cout << "Image non trouvée, in CodeBarre(string)" << std::endl;
    }

    byte* data_img = img.data();
    // extrait le code barre de l'image
    for (int i = 0; i < n; i++)
    {
        data[i] = data_img[i];
    }
}

// ----- methodes vectorielles

CodeBarre CodeBarre::grad() {
    CodeBarre v;

    // gradient suivant +ex;
    for (int i=0;i<n-1;i++) {
        // utilisation de la méthode t() gère le bord
        v(i) = (*this)(i+1)-(*this)(i);
    }
    return v;
}

CodeBarre CodeBarre::div() {
    CodeBarre v;

    for (int i=0;i<n-1;i++) {
        v(i) = (*this)(i+1)-(*this)(i);
    }
    return v;
}

double CodeBarre::normeL1() const {
    double s=0;
    for (int i=0;i<n;i++) {
        s+=abs(data[i]);
    }
    return s;
}

double CodeBarre::normeL2() const{
    // mathématiquement, c'est une norme L2 au carré
    double s=0;
    for (int i=0;i<n;i++) {
        s+=data[i]*data[i];
    }
    return s;
}


// ----- opérateurs

CodeBarre CodeBarre::operator+(const CodeBarre& v) const {
    CodeBarre w(v);
    for (int i=0;i<n;i++) {
        w(i) = w(i) + data[i];
    }
    return w;
}

CodeBarre CodeBarre::operator*(double scalaire) const {
    CodeBarre w;
    for (int i=0;i<n;i++) {
        w(i) = (data[i]*scalaire);
    }
    return w;
}

CodeBarre CodeBarre::operator-(CodeBarre v) {
    CodeBarre w(v);
    for (int i=0;i<n;i++) {
        w(i) = data[i]-w(i);
    }
    return w;
}

CodeBarre CodeBarre::operator*(const CodeBarre& v) const {
    CodeBarre w(v);
    for (int i=0;i<n;i++) {
        w(i) = w(i)*data[i];
    }
    return w;
}


// ----- algorithme

void CodeBarre::normaliser() {
    for (int i=0;i<n;i++) {
        (*this)(i) = (min(max(0,int((*this)(i))),255));

    }
}

CodeBarre CodeBarre::flou(double rayonKer) {
    CodeBarre w;
    int r = int(rayonKer);
    double a = rayonKer-r;
    double fact=0;
    double* Ker = new double[2*r+3];
    for(int k=-r;k<=r;k++) {
        Ker[r+1+k]=(sqrt(pow(rayonKer+0.5,2)-pow(k+0.5,2))+sqrt(pow(rayonKer+0.5,2)-pow(k-0.5,2)));
        fact += Ker[r+1+k];
        cout<<Ker[r+1+k]<<endl;
    }
    Ker[0]=a*sqrt(a*(rayonKer+0.5+r));
    Ker[2*r+2]=a*sqrt(a*(rayonKer+0.5+r));
    fact += 2*a*sqrt(a*(rayonKer+0.5+r));

    for (int i=0; i<n; i++) {
        w(i)=0;
        for(int j=-r-1;j<r+2;j++){
            w(i)+= Ker[r+1+j]*((*this)(i+j));
        }
        w(i)=w(i)/fact;
    }
    delete[] Ker;
    return w;
}

void CodeBarre::etendre(){
    double M=0;
    double m=255;
    for(int i=0;i<largeurImage;i++){
        M = max(M,(*this)(i));
        m = min(m,(*this)(i));
    }
    for(int j=0;j<largeurImage;j++){
        (*this)(j) = (((*this)(j)-m)/(M-m))*255;
    }
    cout<<m<<";"<<" "<<M<<endl;
}

