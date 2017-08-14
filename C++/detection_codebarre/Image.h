#pragma once
#include <Imagine/Graphics.h>
using namespace Imagine;
#include <cstring>
using namespace std;

#include<iostream>
using namespace std;


class Img{
 public:
    int* t;
    int w,h;
    Img();
    Img(int W, int H);
    ~Img();
    void display();
    bool load(string name);
    int get(int x, int y);
    void set(int x, int y, int G);
    void add(int x, int y, int G);
    void gradient(Img& Ix, Img& Iy);
    Img subImage(int xd, int yd, int W, int H);
    void doubleW();
    int sommeX(int x);
    int sommeY(int y);
    int varX(int x, int somme);
    int varY(int y, int somme);
    bool save(string name, int quality);
};
