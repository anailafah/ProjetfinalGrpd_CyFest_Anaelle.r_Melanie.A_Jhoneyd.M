#include "fonct.h"

void viderBuffer(){
  while(getchar()!='\n'){}
}

char* retourneStr(){
char* str=malloc(sizeof(char)*200);
int a;
do{
  printf("\nEntrer le nom de la salle : ");
  //Récupérer une chaine de caractère inférieur à 200 caractères
  a=scanf("%49s",str);
  viderBuffer();
}while(a!=1 || strlen(str)>20);
printf("\n");
return str;
}

Date HeureLocale() {
  //récupération de la date acutelle en secondes
  time_t tempsbase = time(NULL); 
  Date d;
  //transformation des secondes en date explotable sous forme de structure
  struct tm* dateloc = localtime(&tempsbase); 
  //création d'un tableau pour récupérer toutes les données dans un me endroit
  char* dateecrite = malloc(sizeof(char) * 20); 
  //récupération de ces données pour les mettre dans le tableau
  strftime(dateecrite, 20, "%d/%m/%Y %H:%M:%S", dateloc);
  //convertir ce tableau dans une structure qui est en accord avec le reste du programme
  sscanf(dateecrite,"%d/%d/%d %d:%d",&(d.jour),&(d.mois),&(d.annee),&(d.h),&(d.min));
  return d;
}


int verifDate(Date dloc, Date dcon){
  //verification que la date de la fin du concert n'est pas déjà passée
  if(dloc.annee<dcon.annee){
    return 1;
  }
  else if(dloc.annee==dcon.annee){
    if(dloc.mois<dcon.mois){
      return 1;
    }
    else if(dloc.mois==dcon.mois){
      if(dloc.jour<dcon.jour){
        return 1;
      }
      else if(dloc.jour==dcon.jour){
        if(dloc.h<dcon.h){
          return 1;
        }
        else if(dloc.h==dcon.h){
          if(dloc.min<dcon.min){
            return 1;
          }
        }
      }
    }
  }
  return -1;
}       


void AfficheSalleSF(Salle* s){
  int nbr=0;
  int aide = 0;
  int aide2 = 0;
  int aide3=0;
  int aide4=0;
  int k=0;
  //Vérification que la salle n'est pas vide
  if(s==NULL){
    printf("Aucune donnée dans cette salle");
  }   
  else{
    printf("\nPlan : \n\n");
    //Centrer la scène
    for(int i=0;i<(s->ntotcolonne);i++){
      printf(" ");
    }
    printf("SCENE");
    printf("\n\n");
    //Faire une ligne proportionnelle au nombre de colonne, séparation de la scène avec les places
    for(int i=0;i<((s->ntotcolonne)*2 + 6);i++){
      printf("_");
    }
    printf("\n\n");
    printf("     ");
    //afficher le numéro des colonnes quand le nombre de colonne est inférieur à 10
    if(s->ntotcolonne-1<10){
      for(int i=0;i<(s->ntotcolonne-1);i++){
        printf("%d ",i+1);
      }  
    }
    //afficher le numéro des colonnes quand le nombre de colonne est inférieur à 20 (enlever 1 au nombre de colonne car dans la récupération de données on a rajouté 1 pour le \n
    else if(s->ntotcolonne-1<20){
      printf("                  ");
      aide4=(s->ntotcolonne)%10;
      for(int i=1;i<aide4+1;i++){
        printf("1 ");
      }
      printf("\n     ");
      printf("1 2 3 4 5 6 7 8 9 ");
      for(int j=0;j<(s->ntotcolonne - 10);j++){
        printf("%d ",j);
      }
    }  
    //afficher le numéro des colonnes quand le nombre de colonne>20
    else{
    //Ecrire le numéro de chaque colonne en faisant en sorte que le numéro de la dizaine soit au dessus de celui des unités
      printf("                  ");
      for(int i=0; i<(((s->ntotcolonne-1)/10)-2); i++){
        printf("%d %d %d %d %d %d %d %d %d %d ",i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1);
        aide=i+2;
      }
      aide2=(s->ntotcolonne)-(aide*10);
      for(int i=0;i<aide2;i++){
        printf("%d ",aide+1);
      }
      printf("\n");
      printf("   ");
      while(aide3<s->ntotcolonne){
        k=0;
        while(aide3<(s->ntotcolonne) && k<10){
          if(k==0 && aide3==0){
            printf("  ");
          }  
          else{
            printf("%d ",k);
          } 
          k++;
          aide3++;
        }
      } 
    } 
    printf("\n");
    //Affichage des numéros de rangées pour la catégorie A
    for(int i=0;i<s->nligneA;i++){
      if((i+1)<10){
        printf("R %d  ",i+1);
      }
      else{
        printf("R %d ",i+1);
      }
      //affichage des sièges avec leurs couleurs pour la catégorie A
      for(int j=0;j<s->ntotcolonne;j++){
        if(s->tabsiege[i][j]=='o'){
          printf(BLEU "o " REINI);
        }
        else if(s->tabsiege[i][j]=='x'){
          printf(ROUGE "x " REINI);
        }
        else{
          printf(" ");
        }
      }
      printf("\n");
      nbr++;
    }
    //Affichage des numéros de rangées pour la catégorie B
    for(int k=nbr;k<(s->nligneA + s->nligneB);k++){
      if((k+1)<10){
        printf("R %d  ",k+1);
      }
      else{
        printf("R %d ",k+1);
      }
      //affichage des sièges avec leurs couleurs pour la catégorie B
      for(int j=0;j<s->ntotcolonne;j++){
        if(s->tabsiege[k][j]=='o'){
          printf(JAUNE "o " REINI);
        }
        else if(s->tabsiege[k][j]=='x'){
          printf(ROUGE "x " REINI);
        }
        else{
          printf(" ");
        }
      }
      printf("\n");
      nbr++;
    }
    //Affichage des numéros de rangées pour la catégorie C
    for(int z=nbr;z<s->ntotligne;z++){
      if((z+1)<10){
        printf("R %d  ",z+1);
      }
      else{
        printf("R %d ",z+1);
      }
      //affichage des sièges avec leurs couleurs pour la catégorie C
      for(int j=0;j<s->ntotcolonne;j++){
        if(s->tabsiege[z][j]=='o'){
          printf(VERT "o " REINI);
        }
        else if(s->tabsiege[z][j]=='x'){
          printf(ROUGE "x " REINI);
        }
        else{
          printf(" ");
        }
      }
      printf("\n");
    }
    //Affichage des prix
    printf("\n\n");
    printf("LEGENDE :\n");
    printf(BLEU "o - Catégorie A : " REINI);
    printf("%d €\n",s->c.prixA);
    printf(JAUNE "o - Catégorie B : " REINI);
    printf("%d €\n",s->c.prixB);
    printf(VERT "o - Catégorie C : " REINI);
    printf("%d €\n",s->c.prixC);
    printf(ROUGE "x - Sièges réservés\n\n" REINI);
  }
}

void AffcherSalleAF(Salle* s){
  int aide = 0;
  int aide2 = 0;
  int aide3 = 0;
  int aide4 = 0;
  int aide5=0;
  int k=0;

  printf("\nPlan : \n\n");
  for(int i=0;i<(s->ntotcolonne);i++){
    printf(" ");
  }
  printf("SCENE");
  printf("\n\n");
  for(int i=0;i<((s->ntotcolonne)*2 + 6);i++){
    printf("_");
  }
  printf("\n\n");
  for(int i=0;i<(s->ntotcolonne-2);i++){
    printf(" ");
  }
  printf("FOSSE : %d\n",s->c.fosse);

  for(int i=0;i<((s->ntotcolonne)*2 + 6);i++){
    printf("_");
  }
  printf("\n\n");
  printf("     ");

  //Même système d'affichage des rangées que pour la fonction AffichageSalleSF

  if(s->ntotcolonne-1<10){
    for(int i=0;i<(s->ntotcolonne-1);i++){
      printf("%d ",i+1);
    }  
  }
  else if(s->ntotcolonne-1<20){
    printf("                  ");
    aide4=(s->ntotcolonne)%10;
    for(int i=1;i<aide4+1;i++){
      printf("1 ");
    }
    printf("\n     ");
    printf("1 2 3 4 5 6 7 8 9 ");
    for(int j=0;j<(s->ntotcolonne - 10);j++){
      printf("%d ",j);
    }
  }    
  else{
    printf("                  ");
    for(int i=0; i<(((s->ntotcolonne-1)/10)-1); i++){
      printf("%d %d %d %d %d %d %d %d %d %d ",i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1);
      aide=i+2;
    }
    aide2=(s->ntotcolonne-1)-(aide*10);
    for(int j=0;j<aide2+1;j++){
      printf("%d ",aide);
    }
    printf("\n");
    printf("   ");
    while(aide3<s->ntotcolonne){
      k=0;
      while(aide3<(s->ntotcolonne) && k<10){
        if(k==0 && aide3==0){
          printf("  ");
        }  
        else{
          printf("%d ",k);
        } 
        k++;
        aide3++;
      }
    } 
  } 
  printf("\n");
  for(int j=s->nligneA;j<(s->nligneA + s->nligneB);j++){
    if((j+1)<10){
      printf("R %d  ",j+1);
    }
    else{
      printf("R %d ",j+1);
    }  
    for(int k=0;k<s->ntotcolonne;k++){
      if(s->tabsiege[j][k]=='o'){
        printf(JAUNE "o " REINI);
      }
      else if(s->tabsiege[j][k]=='x'){
        printf(ROUGE "x " REINI);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
  for(int z=s->nligneB + s->nligneA;z<s->ntotligne;z++){
    if((z+1)<10){
      printf("R %d  ",z+1);
    }
    else{
      printf("R %d ",z+1);
    }
    for(int k=0;k<s->ntotcolonne;k++){
      if(s->tabsiege[z][k]=='o'){
        printf(VERT "o " REINI);
      }
      else if(s->tabsiege[z][k]=='x'){
        printf(ROUGE "x " REINI);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
  printf("\n\n");
  printf("LEGENDE :\n");
  printf("  - FOSSE : %d €\n",s->c.prixA);
  printf(JAUNE "o - Catégorie B : " REINI);
  printf("%d €\n",s->c.prixB);
  printf(VERT "o - Catégorie C : " REINI);
  printf("%d €\n",s->c.prixC);
  printf(ROUGE "x - Sièges réservés\n\n" REINI);
}

void AfficheSalle(Salle* s){
  if(s!=NULL){
    //Distinction entre salle avec fosse et les salles sans fosse
    if(s->fosse == '0'){
      AfficheSalleSF(s);
    }
    else{
      AffcherSalleAF(s);
    }
  }   
}

void afficheConcertF(Salle** s,int nbrsalle){
  Date datelocale;
  int verif = 0;
  if(s==NULL){
    printf("Aucune salle ne peut être afficher");
    }
  else if(nbrsalle<=0){
    printf("le nombre de salle est incorrecte");
  }
  else{   
    datelocale=HeureLocale();
    for(int i=0;i<nbrsalle;i++){
      if(s[i]!=NULL){
        verif=verifDate(datelocale,(s[i])->c.heure);
        //Affichage du concert que si il y a un concert et que la date n'est pas déjà passée
        if(verif == 1 && s[i]->concert == '1'){
          sleep(1);
          printf("----------------\n");
          //Eciture de la date du concert en ajoutant des 0 si besoin
          if(s[i]->c.heure.h<10){
            if(s[i]->c.heure.min<10){
          printf("\n%d - %s\nLe %d/%d/%d à 0%d:0%d",i+1, s[i]->c.artiste , s[i]->c.heure.jour, s[i]->c.heure.mois, s[i]->c.heure.annee , s[i]->c.heure.h , s[i]->c.heure.min);
            }
            else{
              printf("\n%d - %s\nLe %d/%d/%d à 0%d:%d",i+1, s[i]->c.artiste , s[i]->c.heure.jour, s[i]->c.heure.mois, s[i]->c.heure.annee , s[i]->c.heure.h , s[i]->c.heure.min); 
            }
          }
          else if(s[i]->c.heure.min<10){
            printf("\n%d - %s\nLe %d/%d/%d à %d:0%d",i+1, s[i]->c.artiste , s[i]->c.heure.jour, s[i]->c.heure.mois, s[i]->c.heure.annee , s[i]->c.heure.h , s[i]->c.heure.min);
          }
          else{
            printf("\n%d - %s\nLe %d/%d/%d à %d:%d",i+1, s[i]->c.artiste , s[i]->c.heure.jour, s[i]->c.heure.mois, s[i]->c.heure.annee , s[i]->c.heure.h , s[i]->c.heure.min);
          }
          printf(" dans la salle %s",s[i]->nom);
            AfficheSalle(*(s+i));
        }
      }   
    }
  }   
}


void afficheNomsalle(){
  FILE* nomsalle = fopen("nomsalle.txt","r");
  int nbr=0;
  char recup[200];
  if(nomsalle==NULL){
    printf("20 - Erreur d'ouverture de fichier");
    return;
  }
  nbr=recupNombreSalle(nomsalle);
  printf("\nNom des salles existantes : \n\n");
  for(int i=0;i<nbr-1;i++){
    printf("%d - ",i+1);
    fgets(recup,199,nomsalle);
    printf("%s",recup);
  }
  printf("\n");
}

//Afficher les salles où il n'y a pas de concert
//Même fonctionnement que pour les autres affichages de salles
void AfficheSalleSC(Salle* s){
  int nbr=0;
  int aide = 0;
  int aide2 = 0;
  int aide3 = 0;
  int aide4 = 0;
  int k;


  printf("Plan : \n\n");
  for(int i=0;i<(s->ntotcolonne-2);i++){
    printf(" ");
  }
  printf("SCENE");
  for(int i=0;i<(s->ntotcolonne-3);i++){
    printf(" ");
  }
  printf("\n\n");
  for(int i=0;i<((s->ntotcolonne)*2 + 2);i++){
    printf("_");
  }

  printf("\n\n");
  printf("     ");

  //Même système d'affichage des rangées que pour la fonction AffichageSalleSF

  if(s->ntotcolonne-1<10){
    for(int i=0;i<(s->ntotcolonne-1);i++){
      printf("%d ",i+1);
    }  
  }
  else if(s->ntotcolonne-1<20){
     printf("                  ");
     aide4=(s->ntotcolonne)%10;
     for(int i=1;i<aide4+1;i++){
        printf("1 ");
     }
     printf("\n     ");
     printf("1 2 3 4 5 6 7 8 9 ");
     for(int j=0;j<(s->ntotcolonne - 10);j++){
       printf("%d ",j);
     }

  }    
  else{
   printf("                  ");
   for(int i=0; i<(((s->ntotcolonne-1)/10)-1); i++){
      printf("%d %d %d %d %d %d %d %d %d %d ",i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1,i+1);
      aide=i+2;
   }
   aide2=(s->ntotcolonne-1)-(aide*10);


   for(int j=0;j<aide2+1;j++){
      printf("%d ",aide);
   }

   printf("\n");
   printf("   ");
   while(aide3<s->ntotcolonne){
      k=0;
      while(aide3<(s->ntotcolonne) && k<10){
          if(k==0 && aide3==0){
             printf("  ");
          }  
          else{
              printf("%d ",k);
          } 
          k++;
          aide3++;
      }

   } 
  } 

  printf("\n");

  for(int i=0;i<s->nligneA;i++){
    if((i+1)<10){
      printf("R %d  ",i+1);
    }
    else{
      printf("R %d ",i+1);
    }
    for(int j=0;j<s->ntotcolonne;j++){
      if(s->tabsiege[i][j]=='o'){
        printf(BLEU "o " REINI);
      }
      else if(s->tabsiege[i][j]=='x'){
        printf(ROUGE "x " REINI);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
    nbr++;
  }
  for(int k=nbr;k<(s->nligneA + s->nligneB);k++){
    if((k+1)<10){
      printf("R %d  ",k+1);
    }
    else{
      printf("R %d ",k+1);
    }
    for(int j=0;j<s->ntotcolonne;j++){
      if(s->tabsiege[k][j]=='o'){
        printf(JAUNE "o " REINI);
      }
      else if(s->tabsiege[k][j]=='x'){
        printf(ROUGE "x " REINI);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
    nbr++;
  }
  for(int z=nbr;z<s->ntotligne;z++){
    if((z+1)<10){
      printf("R %d  ",z+1);
    }
    else{
      printf("R %d ",z+1);
    }
    for(int j=0;j<s->ntotcolonne;j++){
      if(s->tabsiege[z][j]=='o'){
        printf(VERT "o " REINI);
      }
      else if(s->tabsiege[z][j]=='x'){
        printf(ROUGE "x " REINI);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
  printf("\n\n");
  printf("LEGENDE :\n");
  printf(BLEU "o - Catégorie A  " REINI);
  printf(JAUNE "o - Catégorie B  " REINI);
  printf(VERT "o - Catégorie C  " REINI);
  printf(ROUGE "x - Sièges réservés\n\n" REINI);
  printf("AUCUN CONCERT DANS CETTE SALLE\n\n");
}

//Fonction générale de l'affichage des salles pour le côté Manager

void AfficheSalleM(Salle* s){
  int verifdate=0;
  if(s->concert == '1' && s->fosse == '0'){
    if(s->c.heure.h<10){
      if(s->c.heure.min<10){
        printf("\nDate - %d/%d/%d à 0%d:0%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
      }
      else{
        printf("\nDate - %d/%d/%d à 0%d:%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min); 
      } 
    }
    else if(s->c.heure.min<10){
      printf("\nDate - %d/%d/%d à %d:0%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
    }
    else{
      printf("Date - %d/%d/%d à %d:%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
    }
    verifdate=verifDate(HeureLocale(),s->c.heure);
    printf("Artiste : %s\n", s->c.artiste);
    if(verifdate!=1){
       printf("Le concert est fini, vous pouvez le modifier\n\n");
    }
    AfficheSalleSF(s);
   }
  else if(s->concert == '1' && s->fosse == '1') {
    if(s->c.heure.h<10){
      if(s->c.heure.min<10){
        printf("\nDate - %d/%d/%d à 0%d:0%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
      }
      else{
        printf("\nDate - %d/%d/%d à 0%d:%d\n\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min); 
      } 
    }
    else if(s->c.heure.min<10){
      printf("\nDate - %d/%d/%d à %d:0%d\n\n" , s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
    }
    else{
      printf("Date - %d/%d/%d à %d:%d\n\n" , s->c.heure.jour, s->c.heure.mois, s->c.heure.annee , s->c.heure.h , s->c.heure.min);
    }
    verifdate=verifDate(HeureLocale(),s->c.heure);
    printf("Artiste : %s\n", s->c.artiste);
    if(verifdate!=1){
      printf("Le concert est fini, vous pouvez le modifier\n\n");
    }
    AffcherSalleAF(s);
  }
  else if(s->concert == '0'){
    AfficheSalleSC(s);
  }
}

int verifSalle(Salle*s){
  if(s==NULL){
    printf("\n17 - Erreur d'allocation ou salle non existante\n\n");
    return 0;
  }
  return 1;
}

int choixMode(){
  int a,choix;
  do{
    printf("1 - Mode Manager\n\n 2 - Mode Festivalier\n\n  3 - Sortir \n\nVotre choix: ");
    a=scanf("%d",&choix);
    viderBuffer();
    printf("\n");
  }while(a!=1 || choix<1 || choix>3);
  return choix;
}
