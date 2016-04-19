#pragma once

#include <cmath>
#include<iostream>
using namespace std;
#include<Imagine/Graphics.h>
using namespace Imagine;

class Perceptron {
public:
    int n; //Taille de w
    double theta; //Seuil
    double* w;
    Perceptron();
    Perceptron(int n0);
    ~Perceptron();
    void set(double ww[], double ttheta); //Regler w a partir d'un ww
    int size() const;
    double seuil();
    double operator[](int i); //renvoie w[i]
    double Heaviside(double I[]); //renvoie Heaviside(w.I - theta)
    double Adaline(double I[]); //renvoie (w.I - theta)
};

class Echantillon {
    int p; //Taille de l'echantillon

    int k; //Taille d'une sortie
    double** el; //Tableau des entrees
    double** v; //Tableau des sorties
    int n; //Taille d'un exemple (i.e. une entree)
public:
    Echantillon(int p0, int n0, int k0);
    ~Echantillon();
    int size() const;
    void set(int i, double ele[], double retour[]); //réglage de l'entree et de la sortie d'indice i
                                                    //a partir d'une entree et d'une sortie donnees
    void setEl(int i, int j, double d); //reglage de la case d'indice j de l'entree d'indice i de l'echantillon
    void setT(int i, int j, double d); //reglage de la case d'indice j de la sortie d'indice i de l'echantillon
    double* getEl(int i); //renvoyer l'entree d'indice i de l'echantillon
    double* tabT(int i); //renvoyer la sortie attendue d'indice i de l'echantillon
    double T(int i, int j); //renvoie l'element d'indice j de la sortie d'indice i de l'echantillon
    double O(int i, Perceptron P); //calcule la reponse du perceptron a l'entree d'indice i de l'echantillon
};


class Reseau{
    int q;
    double** z; // tableau de sorties de perceptrons
    int* nLmax;
    double** derE; //Tableau des dE/dyj
public:
    Perceptron** p; //Tableau des perceptrons
    Reseau();
    void set(int q0, int nL[]); // à utiliser après initialisation de R
    ~Reseau();
    void calculeZ(double I[]); //met a jour tous les z apres une entree I
    double* retourSortie(); //renvoie la sortie stockee par le reseau
    void majP(double T[], double eps); //met a jour les perceptrons
    void apprend(Echantillon E, double eps); //algorithme d'apprentissage
    void affiche();
    void ecritureFichier(ofstream& fileNet);
};


double sigmaR(double x);
