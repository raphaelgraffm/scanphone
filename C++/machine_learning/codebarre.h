#ifndef CODEBARRE_H
#define CODEBARRE_H
#include<Imagine/Graphics.h>
using namespace Imagine;
#include <iostream>
using namespace std;


const int nbBarre = 13; // EAN13
const int resolution = 6;
const int largeurImage = nbBarre*7 + 5*5;

// Classe CodeBarre unidimensionnelle

class CodeBarre {
    // largeur du tableau
    int n;

    // tableau de pixels entre 0 et 255
    double* data;
public:
    CodeBarre();

    // accesseur
    double& operator()(int i);
    int getSize() const;

    // constructeurs
    CodeBarre(int n0, float valInitiale);
    CodeBarre(int n0);
    CodeBarre(double d[], int n0);
    CodeBarre(const CodeBarre& u);
    CodeBarre(const char* chemin);
    ~CodeBarre();

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
    void seuil(double seuil);

    // options
    void augmenterResolution(int res);
};

void affiche(CodeBarre& u, int y, int h);
void affiche(CodeBarre& u);

#endif // CODEBARRE_H
