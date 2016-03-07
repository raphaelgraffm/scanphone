#include <iostream>
using namespace std;
#include "codebarre/codebarre.h"
#include "gradient/gradient.h"
#include <Imagine/Graphics.h>
using namespace Imagine;

const int zoom = 4;
const int l=(largeurImage+1)*zoom;


//void affiche(CodeBarre u) {
  //  for (int i=0;i<largeurImage;i++) {
      //  fillRect(i*zoom,0,zoom);
    //}
//}

int main() {
    int w, h;
    byte* t;
    loadGreyImage(srcPath("imagesCodeBarre/cb1.jpg"), t, w, h);
    openWindow(w,h);
    putGreyImage(0,0,t,w,h);
    click();
    clearWindow();
    int p = int(h/2);
    cout<<w<<endl;
    int tab[largeurImage];
    for(int i=0;i<largeurImage;i++){
        tab[i]=t[p*w+i];
    }
    CodeBarre u0(tab);
    CodeBarre u = u0.flou(20);
    for(int k=0; k<h;k++){
        for(int j=0;j<w;j++){
            t[k*w+j]=byte(u(j));
        }
    }
    putGreyImage(0,0,t,w,h);
    click();
    clearWindow();
    gradient(u,10,0.5,20,u,0.5);
    for(int k=0; k<h;k++){
        for(int j=0;j<w;j++){
            t[k*w+j]=byte(u(j));
        }
    }
    putGreyImage(0,0,t,w,h);
    click();
    endGraphics();
    return 0;
}
