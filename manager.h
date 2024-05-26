#ifndef MANAGER_H
#define MANAGER_H
#include "header1.h"
#include "fonct.h"
#include "festivalier.h"

void creerSalle();
void attribuerC();
Salle*recupInfoM(char* recup);
void ecrireNom(Salle**tabSalle,int taille);
void ecrireSalles(Salle* s);
Date creerDate();
Concert creerConcert();
void creerPlusieursSalles();
int choixFosse();
void attribuer2C();
void attribuer1C(Salle*s);
void attribuerC();
int nvConcert();
void changeCategorie(Salle*s);
void suppC(Salle*s);
float ratioSF(Salle*s1);
float ratioAF(Salle*s1);
void ratio(Salle*s);
int modeM_choix();
void Manager();

#endif
