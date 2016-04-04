// Imagine++ project
// Project:  File
// Author:   Pascal Monasse

#include "Neurone.h"

#include<Imagine/Graphics.h>
using namespace Imagine;
using namespace std;




int main() {

    //---Apprentissage du XOR---
    //Programmation de l'échantillon
    Echantillon E(1000,3,3);
    double a,b,c;
    for (int i=0; i<1000; i++){
        a = double(rand()%2);
        b = double(rand()%2);
        if (a+b==1.)
            c = 1.;
        else
            c = 0.;
        E.setEl(i,1,a);
        E.setEl(i,2,b);
        E.setEl(i,3,0.);
        E.setT(i,1,c);
        E.setT(i,2,c);
        E.setT(i,3,c);
    }

    //Creation du reseau

    Reseau R(4,3);
    R.apprend(E, 0.75);

    for (int L=0; L<3; L++)
        for (int i=0; i<2; i++)
            for (int j=0; j<2; j++)
                cout << "p( " << L << " , " << i << " , " << j << " ): " << R.p[L][i].w[j] << endl;

    //Tests
    double entree[2];
    double* sortie;
    for (int i=0; i<20; i++){
        entree[0] = double(rand()%2);
        entree[1] = double(rand()%2);
        R.calculeZ(entree);
        sortie = R.retourSortie();
        cout << entree[0] << " + " << entree[1] << " = " << sortie[0] << " , " << sortie[1] << endl;
    }

    return 0;
}
