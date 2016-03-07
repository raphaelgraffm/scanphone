#include "codebarre.h"
#include <iostream>

CodeBarre::CodeBarre() {
    // constructeur vide
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=0;
    }
}
int& CodeBarre::operator()(int i) {
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

/**
 * Cree un code barre à partir d'un fichier image
 * On suppose que l'image a une largeur de largeurImage
 */
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

CodeBarre::CodeBarre(int d[largeurImage]) {
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=d[i];
    }
}

// ----- opérateurs

CodeBarre CodeBarre::operator+(const CodeBarre& v) const {
    CodeBarre w(v);
    for (int i=0;i<n;i++) {
        w(i) = w(i) + data[i];
    }
    return w;
}

CodeBarre CodeBarre::operator*(int scalaire) const {
    CodeBarre w;
    for (int i=0;i<n;i++) {
        w(i) = data[i]*scalaire;
    }
    return w;
}

CodeBarre CodeBarre::operator-(const CodeBarre& v) const {
    CodeBarre w(v);
    return w+v*(-1);
}

CodeBarre CodeBarre::operator*(const CodeBarre& v) const {
    CodeBarre w(v);
    for (int i=0;i<n;i++) {
        w(i) = w(i)*data[i];
    }
    return w;
}

CodeBarre::CodeBarre(const CodeBarre& u) {
    n=largeurImage;
    for(int i=0;i<n;i++) {
        data[i]=u.data[i];
    }
}

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

int CodeBarre::normeL1() const {
    int s=0;
    for (int i=0;i<n;i++) {
        s+=data[i];
    }
    return s;
}

int CodeBarre::normeL2() const {
    // mathématiquement, c'est une norme L2 au carré
    int s=0;
    for (int i=0;i<n;i++) {
        s+=data[i]*data[i];
    }
    return s;
}


CodeBarre CodeBarre::flou(int rayonKer) {
    CodeBarre w;

    int fact=2*rayonKer + 1;

    // s initial
    int s=0;
    for(int k=-rayonKer;k<=rayonKer;k++) {
        s+=(*this)(k);
    }
    w(0) = s/fact;

    for (int i=1; i<n; i++) {
       s = s + (*this)(i+rayonKer) - (*this)(i-1-rayonKer);
       w(i) = s/fact;
    }

    return w;
}
