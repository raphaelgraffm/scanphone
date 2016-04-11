// Imagine++ project
// Project:  File

#include "Neurone.h"

#include<Imagine/Graphics.h>
using namespace Imagine;
using namespace std;


int main() {

    //---Apprentissage du AND---
    //Programmation de l'échantillon

    int nEch = 1000;
    Echantillon E(nEch,2,2);
    double a,b,c;
    for (int i=0; i<nEch; i++){
        a = double(rand()%2);
        b = double(rand()%2);
        if (a+b>0) {
            c = 1.;
        }
        else
            c = 0.;
        E.setEl(i,0,a);
        E.setEl(i,1,b);
        E.setT(i,0,c);
        E.setT(i,1,c);
    }


    //Creation du reseau

    Reseau R(2,2);
    cout << "\n\n Initialisation"<< endl;
    R.affiche();
    cout << "\n\n Apprentissage"<< endl;
    R.apprend(E,1.0);
    R.affiche();


    cout << "\n\n Test"<< endl;
    //Tests
    for (int i=0; i<2; i++){
        for(int j=0;j<2;j++) {
            double entree[2];
            entree[0] = i;
            entree[1] = j;
            R.calculeZ(entree);
            double* sortie = R.retourSortie();
            cout << entree[0] << " AND " << entree[1] << " = " << sortie[0] << " , " << sortie[1] << endl;
        }
    }

    return 0;
}
