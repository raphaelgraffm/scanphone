#ifndef CODEBARRE_H
#define CODEBARRE_H
#include<Imagine/Graphics.h>
using namespace Imagine;
#include <iostream>
using namespace std;

// Largeur de l'image analysée en pixels
const int largeurImage=391;

// Classe CodeBarre unidimensionnelle

class CodeBarre {
    // tableau de pixels entre 0 et 255
    double data[largeurImage];
    // largeur du tableau
    int n;
public:
    CodeBarre();

    // accesseur
    double& operator()(int i);

    // constructeurs
    CodeBarre(int valInitiale);
    CodeBarre(int d[largeurImage]);
    CodeBarre(const CodeBarre& u);
    CodeBarre(const char* chemin);

    // methodes vectorielles
    CodeBarre grad();
    CodeBarre div();
    double normeL1() const;
    double normeL2() const; // norme L2 au carré

    // opérateurs
    CodeBarre operator+(const CodeBarre& v) const;
    CodeBarre operator*(double scalaire) const;
    CodeBarre operator-(CodeBarre v);
    CodeBarre operator*(const CodeBarre& v) const; // multiplication terme à terme

    // algorithme
    void normaliser();
    CodeBarre flou(double rayonKer);
    void etendre();
};

#endif // CODEBARRE_H
