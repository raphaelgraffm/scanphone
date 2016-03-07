#ifndef CODEBARRE_H
#define CODEBARRE_H

#include <Imagine/Images.h>
using namespace Imagine;

// Largeur de l'image analysée en pixels
const int largeurImage = 500;

class CodeBarre 
{
    // tableau de pixels entre 0 et 255
    int data[largeurImage];

    // largeur du tableau
    int n;

public:

    CodeBarre();

    // accesseur rapide
    int& operator() (int i);

    // constructeurs
    CodeBarre(int valInitiale);
    CodeBarre(const CodeBarre& u);

    /**
     * Cree un code barre à partir d'un fichier image
     */
    CodeBarre(const char* chemin);
    CodeBarre(int d[largeurImage]);

    // methodes vectorielles
    CodeBarre grad();
    CodeBarre div();
    int normeL1() const;
    int normeL2() const;

    // algorithme
    CodeBarre flou(int rayonKer);

    // opérateurs
    CodeBarre operator+(const CodeBarre& v) const;
    CodeBarre operator*(int scalaire) const;
    CodeBarre operator-(const CodeBarre& v) const;
    CodeBarre operator*(const CodeBarre& v) const; // pour illumination
};

#endif // CODEBARRE_H
