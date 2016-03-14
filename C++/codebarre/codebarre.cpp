#include "codebarre.h"
#include <iostream>
using namespace std;
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;


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
    double fact=2*rayonKer + 1;

    // s initial
    double s=0;
    for(int k=-r;k<=r;k++) {
        s+=(*this)(k);
    }
    s+=a*((*this)(-r-1)+(*this)(r+1));
    w(0) = (s/fact);

    for (int i=1; i<n; i++) {
       s = s + a*(*this)(i+r+1) + (1-a)*(*this)(i+r) + - a*(*this)(i-r-2) - (1-a)*(*this)(i-1-r);
       w(i) = (s/fact);
    }

    return w;
}

void CodeBarre::etendre(){
    double M=0;
    double m=255;
    for(int i=0;i<n;i++){
        M = max(M,(*this)(i));
        m = min(m,(*this)(i));
    }
    for(int j=0;j<n;j++){
        (*this)(j) = (((*this)(j)-m)/(M-m))*255;
    }
}

void CodeBarre::seuil(double seuil){
    for(int i=0;i<n;i++){
        if(data[i]<seuil)
            data[i] = 0;
        else
            data[i] = 255;
    }
}


void affiche(CodeBarre u, int y, int h) {
    for (int i=0;i<largeurImage;i++) {
        Color col(u(i));
        fillRect(i,y,1,h,col);
    }
}

