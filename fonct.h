#ifndef FONCT_H
#define FONCT_H
#include "header1.h"
#include "festivalier.h"
void viderBuffer();
char* retourneStr();
int verifDate(Date dloc, Date dcon);
Date HeureLocale();
void AfficheSalleSF(Salle* s);
void AffcherSalleAF(Salle* s);
void AfficheSalle(Salle* s);
void afficheConcertF(Salle** s,int nbrsalle);
void afficheNomsalle();
void AfficheSalleSC(Salle* s);
void AfficheSalleM(Salle* s);
int verifSalle(Salle*s);
int choixMode();

#endif
