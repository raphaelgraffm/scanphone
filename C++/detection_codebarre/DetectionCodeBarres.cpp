#include <cstdlib>
#include <ctime>
using namespace std;

#include <list>
#include<string>
#include <complex>
#include <cassert>
using namespace std;

#define USE_MATH_DEFINES
#include <cmath>

#include <cassert>
#include<iostream>
using namespace std;

#include<Imagine/Graphics.h>
using namespace Imagine;

#include "image.h"

#include <vector>

///////////////////////////////////////////////
/// Otsu

double interclassVar(int N_min, int sum1, int sum2, int N_tot){
    int N_max = N_tot - N_min;
    int mu1 = double(sum1)/double(N_min);
    int mu2 = double(sum2)/double(N_max);
    double w1 = double(N_min)/double(N_tot);
    return w1*(1-w1)*(mu1-mu2)*(mu1-mu2);
}



Img seuilOtsu(Img& J, int mini, int maxi){
    int W = J.w;
    int H = J.h;

    int seuil = 0, var_max = 0;
    Img J2(W,H);

    //Histogramme: H[i] = nb de pixels de J valant i+mini
    int Histo[maxi-mini+1];
    for (int i=0; i<maxi-mini+1; i++)
        Histo[i] = 0;
    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++)
            Histo[J.get(x,y)-mini]++;

    //Otsu
    int N_min = 0, sum1 = 0, sum2 = 0;
    for (int i=0; i<maxi-mini+1; i++)
        sum2 += (i+mini)*Histo[i];


    for (int t=0; t<maxi-mini+1; t++){
        double K = interclassVar(N_min, sum1, sum2, W*H);
        if (K > var_max)
            {seuil = t;
            var_max = K;}
        N_min += Histo[t];
        sum1 += (t+mini)*Histo[t];
        sum2 -= (t+mini)*Histo[t];}


    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++){
            if (J.get(x,y) < seuil)
                J2.set(x,y, mini );
            else
                J2.set(x,y, maxi);
            }


    return J2;
}

Img Dilate(Img& J, int d){
    Img J2(J.w, J.h);
    for (int xc=0; xc<J.w; xc++)
        for (int yc=0; yc<J.h; yc++)
            if (J.get(xc,yc) == 255)
                for (int x=max(xc-d,0); x<=min(xc+d,J.w-1); x++)
                        J2.set(x,yc, 255 );
    return J2;
}

void Marque(Img& J, Img& J2, int xr, int yr, std::vector<int>& Vx, std::vector<int>& Vy, int i){
    J2.set(xr,yr,i);
    Vx.push_back(xr);
    Vy.push_back(yr);
    int W=J.w;
    int H=J.h;
    int x, y;
    std::vector<int> Vreadx, Vready;
    Vreadx.push_back(xr);
    Vready.push_back(yr);
    while (!Vreadx.empty())
    {
    x = Vreadx.back();
    y = Vready.back();
    Vreadx.pop_back();
    Vready.pop_back();

    if (x>0){
        if (y>0 && J.get(x-1,y-1) == 255 && J2.get(x-1,y-1) != i)
            {Vx.push_back(x-1);
            Vy.push_back(y-1);
            J2.set(x-1,y-1, i);
            Vreadx.push_back(x-1);
            Vready.push_back(y-1);
        }
        if (y<H-1 && J.get(x-1,y+1) == 255 && J2.get(x-1,y+1) != i)
            {Vx.push_back(x-1);
            Vy.push_back(y+1);
            J2.set(x-1,y+1, i);
            Vreadx.push_back(x-1);
            Vready.push_back(y+1);
        }
        if (J.get(x-1,y) == 255 && J2.get(x-1,y) != i)
            {Vx.push_back(x-1);
            Vy.push_back(y);
            J2.set(x-1,y, i);
            Vreadx.push_back(x-1);
            Vready.push_back(y);
            }
        }
    if (x<W-1){
        if (y>0 && J.get(x+1,y-1) == 255 && J2.get(x+1,y-1) != i)
            {Vx.push_back(x+1);
            Vy.push_back(y-1);
            J2.set(x+1,y-1, i);
            Vreadx.push_back(x+1);
            Vready.push_back(y-1);
            }
        if (y<H-1 && J.get(x+1,y+1) == 255 && J2.get(x+1,y+1) != i)
            {Vx.push_back(x+1);
            Vy.push_back(y+1);
            J2.set(x+1,y+1, i);
            Vreadx.push_back(x+1);
            Vready.push_back(y+1);
            }
        if (J.get(x+1,y) == 255 && J2.get(x+1,y) != i)
            {Vx.push_back(x+1);
            Vy.push_back(y);
            J2.set(x+1,y, i);
            Vreadx.push_back(x+1);
            Vready.push_back(y);
            }
        }
    if (y>0 && J.get(x,y-1) == 255 && J2.get(x,y-1) != i)
        {Vx.push_back(x);
        Vy.push_back(y-1);
        J2.set(x,y-1, i);
        Vreadx.push_back(x);
        Vready.push_back(y-1);
        }
    if (y<H-1 && J.get(x,y+1) == 255 && J2.get(x,y+1) != i)
        {Vx.push_back(x);
        Vy.push_back(y+1);
        J2.set(x,y+1, i);
        Vreadx.push_back(x);
        Vready.push_back(y+1);
        }
    }
}

void vide(std::vector<int>& Vtemp){
    while (!Vtemp.empty())
        Vtemp.pop_back();
}

void videNoir(Img& J, std::vector<int>& Vrefx, std::vector<int>& Vrefy){
    while (!Vrefx.empty()){
        J.set( Vrefx.back(), Vrefy.back(), 0);
        Vrefx.pop_back();
        Vrefy.pop_back();
    }
}


void NettoiePetit(Img &J, int a, int b){
    Img J2(J.w, J.h);
    int i=0;
    std::vector< std::vector<int> > Vtotx, Vtoty;
    int maxi=0;
    for (int x=0; x<J.w; x++)
        for (int y=0; y<J.h; y++)
            if (J2.get(x,y) == 0 && J.get(x,y) == 255){
                std::vector<int> Vtempx, Vtempy;
                Marque(J, J2, x, y, Vtempx, Vtempy, i);
                if (maxi < Vtempx.size())
                    maxi = Vtempx.size();
                Vtotx.push_back(Vtempx);
                Vtoty.push_back(Vtempy);
                i++;}

    while (!Vtotx.empty()){
        std::vector<int> Vtempx = Vtotx.back();
        std::vector<int> Vtempy = Vtoty.back();
        if (Vtempx.size() <= (a*maxi)/b)
               videNoir(J, Vtempx, Vtempy);
        Vtotx.pop_back();
        Vtoty.pop_back();
        }

}




void Epuration(Img &J){
    Img J2(J.w, J.h);
    int i=0;
    std::vector<int> Vrefx, Vrefy, Vtempx, Vtempy;
    for (int y=0; y<J.h; y++)
        for (int x=0; x<J.w; x++)
            if (J2.get(x,y) == 0 && J.get(x,y) == 255){
                Marque(J, J2, x, y, Vtempx, Vtempy, i);

                if (Vtempx.size() >= Vrefx.size()){
                    videNoir(J, Vrefx, Vrefy);
                    Vrefx = Vtempx;
                    Vrefy = Vtempy;
                    vide(Vtempx);
                    vide(Vtempy);
                }
                else
                    videNoir(J, Vtempx, Vtempy);
                i++;
           }
}

//Réhaussage du contraste
Img RehausseContraste(Img& I){
    Img I2(I.w, I.h);
    for (int x=0; x<I.w; x++)
        for (int y=0; y<I.h; y++){
            if (I.get(x,y) < 123)
                I2.set(x,y, ((15*(I.get(x,y)))/20) );
            else
                I2.set(x,y, 255+(15*(I.get(x,y)-255))/20 );
        }
    return I2;
}


void calculeBords(int& xmin, int& ymin, int& xmax, int& ymax, Img& J){
    xmin = J.w-1;
    ymin = J.h-1;
    xmax = 0;
    ymax = 0;
    for (int x=0; x<J.w; x++)
        for (int y=0; y<J.h; y++)
            if (J.get(x,y) == 255){
                xmin = min(x,xmin);
                xmax = max(x,xmax);
                ymin = min(y,ymin);
                ymax = max(y,ymax);
            }
}

Img RetrecitBlanc(Img& I, Img& Ix, Img& Iy, int seuilE1, int seuilE2, int seuilV){
    int pixmax = 0;
    for (int x=0; x<I.w; x++)
        for (int y=0; y<I.h; y++)
            if (pixmax < I.get(x,y))
                pixmax = I.get(x,y);
    seuilE1 *= pixmax;
    seuilE2 *= pixmax;
    seuilV *= pixmax;
    seuilE1 /= 255;
    seuilE2 /= 255;
    seuilV /= 255;
    //Bord bas
    int somme;
    int ymin=0;
    somme = I.sommeY(ymin);
        while ((somme <= seuilE1*I.w || (Ix.sommeY(ymin) > seuilV*I.w && somme <= seuilE2*I.w)) && ymin < I.h){
            ymin++;
            somme = I.sommeY(ymin);
    }
    //Bord haut
    int ymax=I.h-1;
    somme = I.sommeY(ymax);
        while ((somme <= seuilE1*I.w || (Ix.sommeY(ymax) > seuilV*I.w && somme <= seuilE2*I.w)) && ymax >= 0){
            ymax--;
            somme = I.sommeY(ymin);
    }
    Img I2 = I.subImage(0,ymin,I.w,ymax-ymin+1);
    Img I2y = Iy.subImage(0,ymin,I.w,ymax-ymin+1);

    //Bord gauche
    int xmin=0;
    somme = I2.sommeX(xmin);
        while ((somme <= seuilE1*I2.h || (I2y.sommeX(xmin) > seuilV*I.h && somme <= seuilE2*I2.h)) && xmin < I2.w){
            xmin++;
            somme = I2.sommeX(xmin);
    }
    //Bord droit
    int xmax=I2.w-1;
    somme = I2.sommeX(xmax);
        while ((somme <= seuilE1*I2.h || (I2y.sommeX(xmin) > seuilV*I.h && somme <= seuilE2*I2.h)) && xmax >= 0){
            xmax--;
            somme = I2.sommeX(xmax);
    }
    if (xmax < xmin)
        return I2;
    return I2.subImage(xmin, 0, xmax-xmin+1, I2.h);
}


// Gradient de l'Img I par differences finies, de m flouté, puis de l'Img du code-barres
Img BarCode(Img& I) {
    int W = I.w;
    int H = I.h;
    Img I2 = RehausseContraste(I);
    Img Ix(W,H);
    Img Iy(W,H);
    I2.gradient(Ix,Iy);
    Img m(W,H);
    for (int y=0; y<H; y++)
        for (int x=0; x<W; x++)
            m.set(x,y, abs(Ix.get(x,y))-abs(Iy.get(x,y)) );

    //Box blur
    Img m2(W, H); //m flouté

    int N=0;
    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++){
            m2.set(x,y,m.get(x,y));
            N = 1;
            if (x>0){
                if (y>0){ N += 1; m2.add(x,y, m.get(x-1,y-1));}
                if (y<H-1){ N += 1; m2.add(x,y, m.get(x-1,y+1));}
                N += 1; m2.add(x,y, m.get(x-1,y));
            }
            if (x<H-1){
                if (y>0){ N += 1; m2.add(x,y, m.get(x+1,y-1));}
                if (y<H-1){ N += 1; m2.add(x,y, m.get(x+1,y+1));}
                N += 1; m2.add(x,y, m.get(x+1,y));
            }
            if (y>0)
                { N += 1;
                m2.add(x,y, m.get(x,y-1));}
            if (y<H-1)
                { N += 1; m2.add(x,y, m.get(x,y+1));}
            m2.set(x,y, m2.get(x,y)/N);
        }

    Img m_binaire = seuilOtsu(m2, 0, 255);
    m_binaire.display();
    click();
    NettoiePetit(m_binaire, 1, 23);
    m_binaire.display();
    click();
    Img m_binaire_2 = Dilate(m_binaire, 16);
    m_binaire_2.display();
    click();
    Epuration(m_binaire_2);
    m_binaire_2.display();
    click();
    int xmin, ymin, xmax, ymax;
    calculeBords(xmin, ymin, xmax, ymax, m_binaire_2);
    Img Icoupe = I.subImage(xmin, ymin, xmax-xmin+1, ymax-ymin+1);
    Img Icoupe_x = Ix.subImage(xmin, ymin, xmax-xmin+1, ymax-ymin+1);
    Img Icoupe_y = Iy.subImage(xmin, ymin, xmax-xmin+1, ymax-ymin+1);
    Img Icoupe2 = RetrecitBlanc(Icoupe, Icoupe_x, Icoupe_y, 90, 130, 255/4);
    //Affichage
    fillRect(0,0,I.w, I.h, WHITE);
    Icoupe2.display();
    drawString(100,Icoupe2.h+50,"Image réduite du code-barres", RED, 16);
    click();
    fillRect(10,Icoupe2.h+25,Icoupe2.w,50,WHITE);
    return Icoupe2;
}



bool extractBarcode(string name, bool disp = true){
    Img I;


    if(! I.load(srcPath() + string(name))) {
        std::cout << "Probleme dans le chargement d'images" << std::endl;
        return false;
    }
    if (disp)
        openWindow(I.w, I.h);
    I.display();
    click();

    Img Icoupe2 = BarCode(I);

    //click();
    if (Icoupe2.save(string(srcPath()) + string("TempBarCode.jpg"), 85))
        cout << "Sauvegarde réussie" << endl;
    else{
        cout << "Echec de la sauvegarde" << endl;
        return false;
    }
    return true;
}

bool extractBarcodeGraphics(){
    byte* rgba;
    int w, h;
    if (!loadAlphaColorImage(srcPath("Gr_accueil.png"),rgba,w,h)){
        delete[] rgba;
        return false;}
    openWindow(max(1200,w),max(1000,h));
    putAlphaColorImage(0,0,rgba,w,h);

    Img I;

    char* name;
    cout << "Nom de l'image" << endl;
    cin >> name;
    fillRect(0,0,w,h,WHITE);
    return extractBarcode(name, false);
}


//////////////////////////////////////////////////////////////////////////
// Fonction principale

int main(){
    //Choisir entre les deux lignes suivantes
    //extractBarcode("CodeBarresFlou3.jpg");
    extractBarcodeGraphics();
    endGraphics();
    return 0;


}
