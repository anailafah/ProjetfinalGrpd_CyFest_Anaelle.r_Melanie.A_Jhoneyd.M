#ifndef FESTIVALIER_H
#define FESTIVALIER_H
#include "header1.h"
#include "fonct.h"
#include "manager.h"
int recupNombreSalle(FILE* nomsalle);
Salle* VerifInfo(Salle* s,int a);
Salle*recupInfoF(char* recup);
Salle** recupSalle(int* nbrsalle);
void changerFichierSF(char nom[200],char** tabsiege,int nbrrangee, int nbrcolonne);
void changerFichierAF(char nom[200],int nbrfosse,int nbrrangee);
void choisirPlace(Salle** s, int nombresalle);
void Festivalier();

#endif
