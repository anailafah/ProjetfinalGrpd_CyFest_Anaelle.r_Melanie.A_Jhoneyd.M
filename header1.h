#ifndef HEADER1_H
#define HEADER1_H
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define REINI "\033[0m"
#define ROUGE "\033[31m"
#define VERT "\033[32m"
#define JAUNE "\033[33m"
#define BLEU "\033[34m"
#define BLANC "\033[37m"

typedef struct{
   int jour;
   int mois;
   int annee;
   int h;
   int min;
}Date; 
typedef struct{
   char artiste[200];
   int fosse;
   int prixA;
   int prixB;
   int prixC;
   Date heure;         
}Concert;
typedef struct{
   char nom[200];
   int ntotligne;
   int ntotcolonne;
   int nligneA;
   int nligneB;
   char** tabsiege;
   char fosse;
   char concert;
   Concert c;
}Salle;

#include "festivalier.h"
#include "fonct.h"
#include "manager.h"
#endif
