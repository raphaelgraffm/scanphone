#include <ctime>
#include <iostream>
#include "codebarre/codebarre.h"
#include "Imagine/Graphics.h"

using namespace std;
using namespace Imagine;

const int zoom = 5;
const int l=(largeurImage);
const int h=300;

void affiche(CodeBarre u) {
    for (int i=0;i<largeurImage;i++) {
        Color col(u(i));
        fillRect(i,0,1,h,col);
    }
}

int main() {

    openWindow(l,h);

    /*CodeBarre u(srcPath("test.png"));*/

    CodeBarre u(255);
    srand(time(0));
    for(int i=0;i<largeurImage/zoom;i++) {
        int a = (rand()%2)*255;
        for(int k=0;k<zoom;k++) {
            u(i*zoom+k) = a;
        }
    }

    u=u.flou(10);
    affiche(u);

    /*click();
    int a=0;
    CodeBarre v;
    while(a<30) {
        v = u.flou(a);
        affiche(v);
        milliSleep(100);
        a+=1;
    }*/

    endGraphics();
    return 0;
}
