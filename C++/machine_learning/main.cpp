// Projet Scanphone - Machine Learning
#include "Neurone.h"
#include "codebarre.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

// --- TODO : ---
// PROBLÈME ETRANGE DE DESALLOCATION DE TABLEAU DOUBLE A RÉGLER

// 0 - créer un nouveau réseau avec le nom identifierReseau
// 1 - apprentissage
// 2 - test et profil
const int MODE = 2;

// Paramètres programme
const bool AFFICHAGE_ECHANTILLONS = false; // affichage codes-barre échantillons
const bool LECTURE_FICHIER = !(MODE==0); // lire un réseau depuis un fichier texte dans reseau/
const bool ECRITURE_FICHIER = (MODE==0); // écrire automatiquement le résultat dans un fichier texte, sinon, cela est demandé
const bool APPRENTISSAGE = true && (MODE == 1); // lancer le programme d'apprentissage
const bool PROFIL_RESEAU = (MODE==2); // faire un test plus complet du réseau et afficher courbe de réponse
const string identifierReseau = "double5_quick"; // Modele de réseau choisi à étudier
/*
percep -> un perceptron unique avec 696 entrées, bande de 3.0 à 5.0
double -> 696 100 1 et une bande de 3.0 à 4.0
double_quick -> 696 10 1 et une bande de 3.0 à 4.0
double5_quick -> 696 10 1 et une bande de 4.0 à 5.0
*/

// Constantes d'apprentissage
const int nEch = 100000; // nombre d'échantillons
const float borne_min_reseau = 4.0; // borne inf rayonFlou -> activation réseau
const float borne_max_reseau = 5.0; // borne sup rayonFlou -> activation réseau
const float taux_apprentissage = 0.1;
/*
Pour un perceptron, 0.01
Pour double_quick, 0.1 marche bien, à réduire lorsqu'on veut converger plus finement

 */

string intToString(int i) {
     ostringstream oss;
     oss << i;
     return oss.str();
}

string floatToString(float i) {
     ostringstream oss;
     oss << i;
     return oss.str();
}


int main() {
    srand(unsigned(time(0)));

    if (AFFICHAGE_ECHANTILLONS || PROFIL_RESEAU)
        openWindow(largeurImage*resolution,300);


    // Initialisation
    cout << "-- Initialisation"<< endl;
    Reseau R;
    int nbEntree = 0; // largeur de l'image d'entrée
    int nFichierALire = 0;

    if (LECTURE_FICHIER) {
        // ----------------------------------- CREATION VIA LECTURE D'UN FICHIER
        string nameFileNet = srcPath("reseau/"+identifierReseau+intToString(nFichierALire)+".txt");
        ifstream fileNet(nameFileNet.c_str());

        if (fileNet) {
            // boucle pour déterminer dernier réseau utilisé
            while(fileNet) {
                nFichierALire++;
                fileNet.close();
                nameFileNet = srcPath("reseau/"+identifierReseau+intToString(nFichierALire)+".txt");
                fileNet.open(nameFileNet.c_str());
            }
            fileNet.close();
            nFichierALire--;
            nameFileNet = srcPath("reseau/"+identifierReseau+intToString(nFichierALire)+".txt");
            cout << "Lecture de " << identifierReseau << nFichierALire << ".txt..." << endl;
            fileNet.open(nameFileNet.c_str());

            // lecture du fichier
            string line;
            // Première ligne contient les infos sur le réseau
            // Lecture nbCouches
            getline(fileNet,line);
            int nbCouches = 0;
            for (int k=0;k<line.size();k++) {
                if(line[k]==' ') {
                    nbCouches++;
                }
            }
            cout << "Nombre de couches : " << nbCouches << endl;

            // Construction nLtest
            int* nL = new int[nbCouches+1];
            stringstream lineSS(line);
            string mot;
            int k=0;
            while(getline(lineSS,mot,' ')) {
                nL[k] = stoi(mot); // string to int
                cout << nL[k] << " " ;
                k++;
            }
            nbEntree = nL[0];
            cout << endl;

            // Construction du réseau
            R.set(nbCouches,nL);

            // poids des perceptrons
            // pour toute couche
            for(int L=0;L<nbCouches;L++) {
                // pour tout perceptron de la couche
                for(int k=0;k<nL[L+1];k++) {
                    double* ww = new double[nL[L]]; // tableau de poids
                    double ttheta;

                    getline(fileNet,line);
                    stringstream lineS(line);

                    // pour toute entrée du perceptron
                    for(int l=0;l<nL[L];l++) {
                        getline(lineS,mot,' ');
                        ww[l] = stod(mot); // string to double
                    }
                    // et le seuil
                    getline(lineS,mot,' ');
                    ttheta = stod(mot);
                    R.p[L][k].set(ww,ttheta);
                    delete[] ww;
                }
            }
            delete[] nL;
            fileNet.close();
        }
        else {
            cout << "Erreur à l'ouverture du fichier réseau." << endl;
            fileNet.close();
            return 1;
        }
    }
    else {
        // ----------------------------------- CREATION VIA PARAMÈTRE
        int nbCouches = 2;
        int* nLtest = new int[nbCouches+1];
        nLtest[0] = largeurImage*resolution; // nombre d'entrées
        nbEntree = nLtest[0];
        for(int i=1;i<nbCouches;i++)
            nLtest[i]=10; // neurones couche cachée
        nLtest[nbCouches] = 1; // sorties
        R.set(nbCouches,nLtest);
        delete[] nLtest;
    }
    //R.affiche();



    //Création des l'échantillons
    Echantillon E(nEch,(APPRENTISSAGE) ? nbEntree : 1,1);
    if(APPRENTISSAGE) {
        cout << "-- Création des échantillons"<< endl;
        clock_t t0,t1; // mesure du temps d'apprentissage pour information
        t0 = clock();
        for (int i=0; i<nEch; i++){
            // À ADAPTER EN FONCTION DU FICHIER
            // Code flou aléatoire
            float rayonFlou = float(rand()%100+1)/10;

            CodeBarre cb1(largeurImage);
            cb1.augmenterResolution(resolution);
            CodeBarre cb2 = cb1.flou(rayonFlou);

            if (AFFICHAGE_ECHANTILLONS)
                affiche(cb2,0,300);

            // Entrées
            for(int k=0;k<largeurImage*resolution;k++)
                E.setEl(i,k,cb2(k)/255);

            // Sorties
            if(borne_min_reseau<rayonFlou && rayonFlou<borne_max_reseau)
                E.setT(i,0,1);
            else
                E.setT(i,0,0);
        }
        t1  = clock();
        cout << "Temps de création : " << (float)(t1-t0)/CLOCKS_PER_SEC << " secondes "<< endl;


        // Apprentissage
        cout << "-- Apprentissage"<< endl;
        t0 = clock();
        R.apprend(E,taux_apprentissage); // algorithme de rétro-propagation
        //R.affiche();
        t1 = clock();
        cout << "Temps d'apprentissage : " << (float)(t1-t0)/CLOCKS_PER_SEC << " secondes "<< endl;
    }



    // Sauvegarde du réseau dans un fichier
    if(ECRITURE_FICHIER) {
        string nameFileNet = srcPath("reseau/"+identifierReseau+intToString(nFichierALire+1)+".txt");
        ofstream fileNet(nameFileNet.c_str(), ios::out | ios::trunc);
        if (fileNet) {
            R.ecritureFichier(fileNet);
            fileNet.close();
        }
        else {
            cout << "Erreur à l'ouverture du fichier d'écriture pour le réseau." << endl;
            fileNet.close();
        }
    }



    // Vérification du réseau
    if (!PROFIL_RESEAU) {
        // Profil rapide en format texte
        cout << "-- " << identifierReseau << nFichierALire+1 << endl;
        int nbTest = 200;
        //Tests
        for (float i=1; i<10; (borne_min_reseau-1.0<=i && i<=borne_max_reseau+1.0) ? i=i+0.5 : i=i+1.0){
            float rayonFlou = i;
            double sortieMoyenne = 0;
            for(int j=0; j<nbTest;j++) {
                CodeBarre Cinit(largeurImage);
                Cinit.augmenterResolution(resolution);
                CodeBarre Cflou = Cinit.flou(rayonFlou);
                double entree[largeurImage*resolution];
                for(int k=0;k<largeurImage*resolution;k++) {
                    entree[k]=Cflou(k)/255;
                }
                R.calculeZ(entree);
                double* sortie = R.retourSortie();
                sortieMoyenne += sortie[0];
            }
            sortieMoyenne /= nbTest;
            cout << " Flou d'entrée : " << rayonFlou << " -> " << sortieMoyenne << endl;
        }
    }
    else {
        // Profil complet du réseau
        int nbTest = 500;
        float pasProfil = 0.2;
        //Tests
        for (float i=pasProfil;i<10;i=i+pasProfil){
                float rayonFlou = i;
                double sortieMoyenne = 0;
                for(int j=0; j<nbTest;j++) {
                    CodeBarre Cinit(largeurImage);
                    Cinit.augmenterResolution(resolution);
                    CodeBarre Cflou = Cinit.flou(rayonFlou);
                    double entree[largeurImage*resolution];
                    for(int k=0;k<largeurImage*resolution;k++) {
                        entree[k]=Cflou(k)/255;
                    }
                    R.calculeZ(entree);
                    double* sortie = R.retourSortie();
                    sortieMoyenne += sortie[0];
                }
                sortieMoyenne /= nbTest;
                fillRect(i*65,300-sortieMoyenne*300,pasProfil*65,300,BLUE);
                drawString(i*65,295-sortieMoyenne*300,floatToString(i),BLACK,6);
        }
        click();
    }



    // Sauvegarde du réseau dans un fichier après vérification, si l'option est décochée
    if(!ECRITURE_FICHIER && APPRENTISSAGE) {
        int choix;
        cout << "Voulez-vous sauvegarder le réseau dans "<< identifierReseau << nFichierALire+1 << ".txt ? [1/0]" << endl;
        cin >> choix;
        if (choix==1) {
            string nameFileNet = srcPath("reseau/"+identifierReseau+intToString(nFichierALire+1)+".txt");
            ofstream fileNet(nameFileNet.c_str(), ios::out | ios::trunc);
            if (fileNet) {
                R.ecritureFichier(fileNet);
                cout << "Réseau sauvegardé !" << endl;
                fileNet.close();
            }
            else {
                cout << "Erreur à l'ouverture du fichier d'écriture pour le réseau." << endl;
                fileNet.close();
            }
        }
    }


    if (AFFICHAGE_ECHANTILLONS || PROFIL_RESEAU)
        endGraphics();

    return 0;
}
