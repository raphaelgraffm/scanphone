#include "image.h"

Img::Img(){
    w = 0;
    h = 0;
    t = 0;
}

Img::Img(int W, int H){
    w = W;
    h = H;
    t = new int[W*H];
    for (int k=0; k<W*H; k++)
        t[k] = 0;
}

Img::~Img(){
    delete[] t;}

void Img::display(){
    byte* t2 = new byte[w*h];
    for (int x=0; x<w; x++)
        for (int y=0; y<h; y++)
            t2[x+w*y] = byte(t[x+w*y]);
    putGreyImage(0, 0, t2, w, h);
    delete[] t2;
}
bool Img::load(string name){
    byte* t2;
    if (!loadGreyImage(name,t2,w,h)){
        delete[] t2;
        return false;}
    t = new int[w*h];
    for (int x=0; x<w; x++)
        for (int y=0; y<h; y++)
            t[x+w*y] = int(t2[x+w*y]);
    delete[] t2;
    cout << "Ouverture ok" << endl;
    return true;
}

int Img::get(int x, int y){
    return t[x+w*y];}

void Img::set(int x, int y, int G){
    t[x+w*y] = G;}

void Img::add(int x, int y, int G){
    t[x+w*y] += G;}

//Gradient de I
void Img::gradient(Img& Ix, Img& Iy){
    for (int x=0; x<w; x++){
        for (int y=0; y<h; y++){
            if (x==0)
                Ix.set(x,y,get(x+1,y)-get(x,y));
            else if (x==w-1)
                Ix.set(x,y,get(x,y)-get(x-1,y));
            else
                Ix.set(x,y,(get(x+1,y)-get(x-1,y))/2);
            if (y==0)
                Iy.set(x,y,get(x,y+1)-get(x,y));
            else if (y==h-1)
                Iy.set(x,y,get(x,y)-get(x,y-1));
            else
                Iy.set(x,y,(get(x,y+1)-get(x,y-1))/2);

        }
    }
}

//Sous-image de I
Img Img::subImage(int xd, int yd, int W, int H){
    Img I2(W, H);
    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++){
            assert(xd+x >= 0 && xd+x < w);
            assert(yd+y >= 0 && yd+y < h);
            I2.set(x, y, get(xd+x,yd+y) );}
    return I2;
}

//Sauvegarder I
bool Img::save(string name, int quality = 85){
    byte* t2 = new byte[w*h];
    for (int x=0; x<w; x++)
        for (int y=0; y<h; y++)
            t2[x+w*y] = byte(t[x+w*y]);
    bool b = saveGreyImage(name, t2, w, h, quality);
    delete[] t2;
    return b;
}


void Img::doubleW(){
    int* t2 = new int[2*w*h];
    for (int x=0; x<w; x++)
        for (int y=0; y<h; y++){
            t2[2*w*y + 2*x] = get(x,y);
            t2[2*w*y + 2*x+1] = get(x,y);
        }
    delete[] t;
    t = t2;
    w *= 2;
}

int Img::sommeX(int x){
    int somme = 0;
    for (int y=0; y<h; y++)
        somme += abs(get(x,y));
    return somme;
}

int Img::varX(int x, int somme){
    int var = 0;
    for (int y=0; y<h; y++)
        var += get(x,y)*get(x,y);
    return (h*var-somme*somme)/(h*h);
}

int Img::varY(int y, int somme){
    int var = 0;
    for (int x=0; x<w; x++)
        var += get(x,y)*get(x,y);
    return (h*var-somme*somme)/(h*h);
}
int Img::sommeY(int y){
    int somme = 0;
    for (int x=0; x<w; x++)
        somme += abs(get(x,y));
    return somme;
}
