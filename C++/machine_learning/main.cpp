// Imagine++ project
// Project:  File
// Author:   Pascal Monasse

#include "Neurone.h"
#include<ctime>
#include<Imagine/Graphics.h>
using namespace Imagine;


/* PROBLÈME ETRANGE DE DESALLOCATION DE TABLEAU DOUBLE A RÉGLER */

int main() {
    srand(unsigned(time(0)));

    //---Apprentissage du XOR---
    //Programmation de l'échantillon

    int nEch = 10000;
    Echantillon E(nEch,2,1);
    for (int i=0; i<nEch; i++){
        double a = double(rand()%2);
        double b = double(rand()%2);
        double c;
        if (a+b==1.0)
            c = 1.;
        else
            c = 0.;
        E.setEl(i,0,a);
        E.setEl(i,1,b);
        E.setT(i,0,c);
    }


    //Creation du reseau
    int nbCouches = 3;
    int* nLtest = new int[nbCouches+1];
    nLtest[0] = 2; // nombre d'entrées
    for(int i=1;i<nbCouches;i++)
        nLtest[i]=3;
    nLtest[nbCouches] = 1; // sortie unique

    Reseau R(nbCouches,nLtest);
    cout << "\n Initialisation"<< endl;
    //R.affiche();
    cout << "\n Apprentissage"<< endl;
    R.apprend(E,1.0);
    R.affiche();


    cout << "\n\n Test"<< endl;
    //Tests
    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++) {
            double entree[2];
            entree[0] = double(i);
            entree[1] = double(j);
            R.calculeZ(entree);
            double* sortie = R.retourSortie();
            cout << entree[0] <<" XOR " << entree[1] << " -> " << sortie[0] << endl;
        }
    }

    return 0;
}
