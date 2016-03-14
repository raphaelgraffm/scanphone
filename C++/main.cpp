#include <iostream>
using namespace std;
#include "codebarre/codebarre.h"
#include "gradient/gradient.h"
#include <Imagine/Graphics.h>
using namespace Imagine;

const int zoom = 4;
const int l=(largeurImage+1)*zoom;

int main() {
    // rayon de floutage
    double R = 6;

    // chargement image
    int w, h;
    byte* t;
    loadGreyImage(srcPath("imagesCodeBarre/cb1.jpg"), t, w, h);
    cout<<w<<endl;
    openWindow(w,4*h);
    putGreyImage(0,0,t,w,h);
    click();

    // extraction ligne à analyser
    int tab[largeurImage];
    for(int i=0;i<largeurImage;i++){
        tab[i]=t[int(h/2)*w+i];
    }

    // définition objets code-barre
    CodeBarre u0(tab);
    CodeBarre u=u0.flou(R);
    affiche(u,h,h);
    click();

    // descente de gradient
    gradient(u,0.2,1,R,u,0);
    u.seuil(128);
    affiche(u,2*h,h);
    click();

    // trouver le rayon
    /*CodeBarre ur = u0.flou(R);
    gradientR(ur,0.01,1,2,10,ur,0);
    ur.normaliser();
    for(int k=0; k<h;k++){
        for(int j=0;j<w;j++){
            t[k*w+j]=byte(ur(j));
        }
    }
    Clean(t,w,h,150);
    putGreyImage(0,3*h,t,w,h);
    endGraphics();*/

    return 0;
}



