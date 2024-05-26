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

void creerSalle();
void attribuerC();

void viderBuffer(){
    while(getchar()!='\n'){}
}

char* retourneStr(){
  char* str=malloc(sizeof(char)*50);
  int a;
  do{
    printf("\nEntrer le nom de la salle : ");
    
    //Récupérer une chaine de caractère inférieur à 200 caractères

    a=scanf("%199s",str);
    viderBuffer();
  }while(a!=1 || strlen(str)>20);
  printf("\n");
  return str;
}

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

int recupNombreSalle(FILE* nomsalle){
    int nbr = 0;
    int curseur = 0;
    
    if(nomsalle==NULL){
        printf("Code d'erreur : '%s'",strerror(errno));
        return -1;
    }
    
    //Récupération des nom de salles avec le fichier qui contient tous les noms ligne par ligne
    
    else{
        while((curseur=fgetc(nomsalle))!=EOF){
            if(curseur=='\n'){
            nbr++;
            }
        }
        if(curseur!='\n'){
            nbr++;
        }
        //retour du curseur au début du fichier pour que les autres fonctions utilisent bien le fichier depuis le début
        rewind(nomsalle);
        return nbr-1;
    }
}    

Salle* VerifInfo(Salle* s,int a){

  //Vérification que les données récupérées sont bien cohérentes
  //En 2 if pour éviter qu'il y a trop de conditions dans un if

  if(a==2 || s->ntotcolonne<0 || s->ntotcolonne>99 || s->ntotligne<0 || s->ntotligne>99 || (s->fosse!='0' && s->fosse!='1')){
    s->concert='0';
    s->ntotcolonne=0;
    s->ntotligne=0;
    s->nligneA=0;
    s->nligneB=0;
    s->tabsiege=NULL;
    s->fosse='0';
    s->c.artiste[0]='0';
    s->c.fosse=0;
    s->c.prixA=0;
    s->c.prixB=0;
    s->c.prixC=0;
    s->c.heure.jour=0;
    s->c.heure.mois=0;
    s->c.heure.annee=0;
    s->c.heure.h=0;
    s->c.heure.min=0;
  }
  if(s->nligneA<0 || s->nligneB<0 || s->tabsiege==NULL || s->c.artiste==NULL || s->c.prixA<0 || s->c.prixB<0 || s->c.prixC<0 || (s->concert!='0' && (s->concert!='1'))){
    s->concert='0';
    s->ntotcolonne=0;
    s->ntotligne=0;
    s->nligneA=0;
    s->nligneB=0;
    s->tabsiege=NULL;
    s->fosse='0';
    s->c.artiste[0]='0';
    s->c.fosse=0;
    s->c.prixA=0;
    s->c.prixB=0;
    s->c.prixC=0;
    s->c.heure.jour=0;
    s->c.heure.mois=0;
    s->c.heure.annee=0;
    s->c.heure.h=0;
    s->c.heure.min=0;
  }

  return s;

}
    

Salle*recupInfoF(char* recup){    
  FILE* infosalle = NULL;
  Salle* s = malloc(sizeof(Salle));
  Concert c;
  int verif = 0;
  int verif2 = 0;
  int a = 0;
  char nom[305];
  char saut[300];
  char txt[5]= ".txt";

  if(s==NULL){
     return NULL;
  }

  //Création du chaine de caratères pour mettre le nom du fichier

  strcpy(nom,recup);
  strcat(nom,txt);
  
  //Ouverture du fichier en mode lecture

  infosalle=fopen(nom,"r");
  if(infosalle==NULL){
    printf("Il n'y a pas de concert dans la salle %s\n\n",recup);
    free(infosalle);
    return NULL;
  }

  //Pour récupérer chaque donnée on utilise fseek pour se déplacer de n caractères
  //Utilisation de fgetc quand on veut un caratère, fgets pour une chaine de caractère, et fscanf pour des int

  fseek(infosalle,15,SEEK_SET);
  fgets(s->nom,199,infosalle);

  fseek(infosalle,10,SEEK_CUR);
  s->concert=fgetc(infosalle);

  //Récupération des données que s'il y a un concert
  
  if(s->concert=='1'){
   fgetc(infosalle);
   fseek(infosalle,20,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->ntotligne));


   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,23,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->ntotcolonne));
  

   if(verif!=1){
     a=2;
   }

   //++ car il faut pouvoir stocker le \n à la fin de la ligne où il y a les sièges

   s->ntotcolonne++;

   fgetc(infosalle);
   verif2 = fscanf(infosalle,"Heure : %d/%d/%d %d:%d",&(s->c.heure.jour),&(s->c.heure.mois),&(s->c.heure.annee),&(s->c.heure.h),&(s->c.heure.min));
  
   if(verif2!=5){
     a=2;
   }

   fgets(saut,299,infosalle);
   fseek(infosalle,17,SEEK_CUR);
   fgets(s->c.artiste,199,infosalle);

   fseek(infosalle,22,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->nligneA));
   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,22,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->nligneB));
   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,8,SEEK_CUR);
   s->fosse=fgetc(infosalle);

   fgetc(infosalle);
   fseek(infosalle,15,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->c.fosse));
   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,16,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->c.prixA));
   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,16,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->c.prixB));
   if(verif!=1){
     a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,16,SEEK_CUR);
   verif = fscanf(infosalle,"%d",&(s->c.prixC));
   if(verif!=1){
    a=2;
   }

   fgetc(infosalle);
   fseek(infosalle,7,SEEK_CUR);
   fgetc(infosalle);
   s->tabsiege=malloc(sizeof(char*)*(s->ntotligne));
   if(s->tabsiege==NULL){
     printf("4 - Erreur d'allocation");
     free(s->tabsiege); 
   }

   for(int i=0;i<(s->ntotligne );i++){
     *(s->tabsiege+i)=malloc(sizeof(char)*(s->ntotcolonne));
     if(*(s->tabsiege+i)==NULL){
       printf("5 - Erreur d'allocation");
       free(*(s->tabsiege));
     }
     else{
       for(int j=0;j<s->ntotcolonne;j++){
         s->tabsiege[i][j]=getc(infosalle);
         if(s->tabsiege[i][j]=='\n'){
         j=350;
         }
       }
     }
   }

  } 

  //Fermeture du fichier

  fclose(infosalle);

  //Vérification des données avant de retourner la salle

  s=VerifInfo(s,a);

  return s;
} 

Salle*recupInfoM(char* recup){    
  FILE* infosalle = NULL;
  Salle* s = malloc(sizeof(Salle));
  Concert c;
  int a=0;
  int verif = 0;
  char nom[305];
  char saut[300];
  char txt[5]= ".txt";

  //Fonction identique à recupInfoF mais recupère les données même si il n'y a pas de concert

  if(s==NULL){
     printf("6 - Erreur d'allocation");
     return NULL;
  }

  strcpy(nom,recup);
  strcat(nom,txt);
  infosalle=fopen(nom,"r");

  if(infosalle==NULL){
    printf("Il n'y a pas de concert dans la salle %s\n\n",recup);
    free(infosalle);
    return NULL;
  }

  fseek(infosalle,15,SEEK_SET);
  fgets(s->nom,199,infosalle);

  fseek(infosalle,10,SEEK_CUR);
  s->concert=fgetc(infosalle);

  fgetc(infosalle);
  fseek(infosalle,20,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->ntotligne));

  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,23,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->ntotcolonne));
  s->ntotcolonne++;

  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  verif = fscanf(infosalle,"Heure : %d/%d/%d %d:%d",&(s->c.heure.jour),&(s->c.heure.mois),&(s->c.heure.annee),&(s->c.heure.h),&(s->c.heure.min));
  
  if(verif!=5){
    a=2;
  }

  fgets(saut,299,infosalle);
  fseek(infosalle,17,SEEK_CUR);
  fgets(s->c.artiste,199,infosalle);

  fseek(infosalle,22,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->nligneA));
  
  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,22,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->nligneB));
  
  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,8,SEEK_CUR);
  s->fosse=fgetc(infosalle);

  fgetc(infosalle);
  fseek(infosalle,15,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->c.fosse));
  
  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,16,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->c.prixA));
  
  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,16,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->c.prixB));
  
  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,16,SEEK_CUR);
  verif = fscanf(infosalle,"%d",&(s->c.prixC));

  if(verif!=1){
    a=2;
  }

  fgetc(infosalle);
  fseek(infosalle,7,SEEK_CUR);
  fgetc(infosalle);

  s->tabsiege=malloc(sizeof(char*)*(s->ntotligne));

  if(s->tabsiege==NULL){
    printf("7 - Erreur d'allocation");
    free(s->tabsiege); 
  }

  for(int i=0;i<(s->ntotligne );i++){
    *(s->tabsiege+i)=malloc(sizeof(char)*(s->ntotcolonne));

    if(*(s->tabsiege+i)==NULL){
      printf("8 - Erreur d'allocation");
      free(*(s->tabsiege));
    }

    else{
      for(int j=0;j<s->ntotcolonne;j++){
        s->tabsiege[i][j]=getc(infosalle);
        
        if(s->tabsiege[i][j]=='\n'){
        j=350;
        }
      }
    }
    
  }
  fclose(infosalle);

  s=VerifInfo(s,a);

  return s;
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
       ("Aucune donnée dans cette salle");
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
    
    
Salle** recupSalle(int* nbrsalle){
    Salle** salle = NULL;
    FILE* nomsalle = NULL;
    int nbr=0;
    char recup[300];
    
    //ouverture du fichier des noms de salle en mode lecture
    
    nomsalle=fopen("nomsalle.txt","r");
    
    if(nomsalle==NULL){
       printf("Code d'erreur : '%s'",strerror(errno));
    }  
    
    if(nbrsalle<=0){
      printf("Le nombre de salle n'est pas bon, la récupértion des salles ne va pas se faire");
      return NULL;
    }  
    
    else{
    
        //Récupération du nombre de salles
    
        *nbrsalle=recupNombreSalle(nomsalle);
        
        nbr=*nbrsalle;
        
        salle=malloc(sizeof(Salle*)*nbr);
        
        if(salle==NULL){
          ("9 - Erreur d'allocation");
          return NULL;
        }  
    
        for(int i=0;i<nbr;i++){
            
            fscanf(nomsalle,"%s",recup);
            *(salle+i)=malloc(sizeof(Salle));
            
            if(*(salle+i)==NULL){
               printf("10 - Erreur d'allocation, la récupération de la salle numéro %d ne va pas pouvoir se faire",i+1);
               return NULL;
            }   
            
            //Récupération des informations d'une salle
            
            (salle[i])=recupInfoF(recup);
        }
    }

    //Fermeture du fichier

    fclose(nomsalle);
    
    return salle;
        
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

void changerFichierSF(char nom[200],char** tabsiege,int nbrrangee, int nbrcolonne){
    FILE* infosalle=NULL;
    char recup[300];
    char txt[5]=".txt";
    char nomfichier[305];
    int verif=-1;
    char** donnee = NULL;
    
    if(tabsiege!=NULL && nbrrangee>0 && nbrcolonne>0){
       
       //création du tableau pour récupérer le nom sans le \n
       
       for(int i=0;i<200;i++){
           if(nom[i]!='\n'){
               recup[i]=nom[i];
           }
           else{
               recup[i]='\0';
               i=201;
           }
       }
       
       //Création d'un tableau pour ajouter .txt au nom
       
       strcpy(nomfichier,recup);
       strcat(nomfichier,txt);
       
       //Ouverture du fichier en mode lecture
 
       infosalle=fopen(nomfichier,"r");
       
       if(infosalle==NULL){
           printf("erreur de fichier");
           fclose(infosalle);
       }
       
       else{
       
           //Création d'un tableau 2 dimensions pour récupérer les données du ficher que l'on change pas
           
           donnee=malloc(sizeof(char*)*14);
           
           if(donnee==NULL){
             return;
           }  

           for(int i=0;i<14;i++){
           
           
              *(donnee+i)=malloc(sizeof(char)*300);
              
              if(*(donnee+i)==NULL){
                 return;
              }   
              
              fgets(*(donnee+i),299,infosalle);
           }  
           
           //fermeture du fichier

           fclose(infosalle);
           
           //Ouverture du fichier en mode écriture

           infosalle=fopen(nomfichier,"w");

           if(infosalle==NULL){
             printf("erreur de fichier");
             fclose(infosalle);
           } 
           else{
              
               //Réécriture du fichier avec les données stocker dans le tableau 2 dimensions
           
               for(int i=0;i<14;i++){
                   fputs(*(donnee+i),infosalle);
               }
               
               //Réécriture des sièges avec les modifications du à la prise de place

               for(int i=0;i<nbrrangee;i++){
                   for(int j=0;j<nbrcolonne;j++){
                       if(tabsiege[i][j]=='x' || tabsiege[i][j]=='o'){
                           fputc(tabsiege[i][j],infosalle);
                       }
                       else{
                           fputc('\n',infosalle);
                           j=201;
                       }   
                   }
               }
               
               //fermeture du fichier
               
               fclose(infosalle);


           }
       }
       
       //Liberation de la mémoire du tableau 2 dimensions qui contenait les donnees du fichier
       
       for(int i=0;i<14;i++){
           free(*(donnee+i));
       }
       free(donnee);
    }   

}

void changerFichierAF(char nom[200],int nbrfosse,int nbrrangee){
    FILE* infosalle=NULL;
    char recup[200];
    char txt[5]=".txt";
    char nomfichier[305];
    int verif=-1;
    char** donnee1 = NULL;
    char** donnee2 = NULL;
    char fosse[4];
    
    if(nbrfosse<0 || nbrrangee<=0){
       return;
    }   
    
    //Création du nom pour retrouver le ficher comme dans la fonction changerFichierAF

    for(int i=0;i<200;i++){
        if(nom[i]!='\n'){
            recup[i]=nom[i];
        }
        else{
            recup[i]='\0';
            i=201;
        }
    }
    
    strcpy(nomfichier,recup);
    strcat(nomfichier,txt);
    
    //Ouverture du fichier en mode lecture
    
    infosalle=fopen(nomfichier,"r");
    
    if(infosalle==NULL){
        printf("erreur de fichier");
        fclose(infosalle);
    }

    else{
    
        //Récupération des données du fichier jusqu'à la ligne qui concerne la fosse
    
        donnee1=malloc(sizeof(char*)*9);
        
        if(donnee1==NULL){
           return;
        }   

        for(int i=0;i<9;i++){
        
          *(donnee1 + i)=malloc(sizeof(char)*300);
          
          if(*(donnee1 + i)==NULL){
            return;
          }  
          
          fgets(*(donnee1+i),299,infosalle);
        }
        
        //Bouger le curseur d'une ligne
        
        fgets(recup,199,infosalle);    
        
        //Transformer le nombre de fosse qui a changé en chaine de caractère
        
        sprintf(fosse,"%d",nbrfosse);
        
        //Recupération de toutes les données restantes

        donnee2=malloc(sizeof(char*)*(nbrrangee + 4));
        
        if(donnee2==NULL){
           return;
        }   
        
        for(int i=0;i<(nbrrangee + 4);i++){
        
          *(donnee2 + i)=malloc(sizeof(char)*300);
          
          if(*(donnee2 + i)==NULL){
            return;
          }  
          
          fgets(*(donnee2+i),299,infosalle);
        }
        
        //fermeture du fichier
        
        fclose(infosalle);

        //ouverture du fichier en mode écriture
        
        infosalle=fopen(nomfichier,"w");
        
        //Réécriture des données 

        for(int i=0;i<9;i++){
          fputs(*(donnee1+i),infosalle);
          
          //Libération de la mémoire des premières données
          
          free(*(donnee1 + i));
        }
        
        //Ecriture du nombre de place en fosse
        
        fputs("Nombre fosse : ",infosalle);
        fputs(fosse,infosalle);
        fputc('\n',infosalle);
        
        //Réécriture du reste des données
        for(int i=0;i<(nbrrangee + 4);i++){
          fputs(*(donnee2+i),infosalle);
          
          //Libération de la mémoire des dernières données
          
          free(*(donnee2 + i));
        }
        fputc('\n',infosalle);
        
        //Fermeture du fichier
        
        fclose(infosalle);

    }
      
    //Libération de la mémoire
    free(donnee1);
    free(donnee2);
}
void choisirPlace(Salle** s, int nombresalle){
    int verif=0;
    int num=0;
    int rangee=0;
    int colonne=0;
    int verifchoix=0;
    int choix=0;
    char choixfosse;
    
    if(s==NULL || nombresalle<=0){
       return;
    }   

    //Création d'une ligne de pointillé pour séparer les affichages de salle des questions
    
    printf("-------\n\n");
    do{
        printf("Voulez vous réserver une place ? (1 : oui et 0 : non) ");
        verif=scanf("%d",&choix);
        viderBuffer();
    }while(verif!=1 || (choix!=1 && choix!=0));
    
    printf("\n");

    if(choix==1){
       do{
          printf("Veuillez choisir le concert où vous voulez réserver (numéro du concert) : ");
          verif=scanf("%d",&num);
          viderBuffer();
       }while(verif!=1 || num<=0 || num > nombresalle || verifDate(HeureLocale(),s[num-1]->c.heure)!=1 || s[num-1]->concert!='1');
       
       //Vérification que cette salle n'est pas vide
       
       if(s[num-1]==NULL){
          return;
       }
       
       printf("\n");

       //Séparation des questions selon la présence de la fosse

        if(s[num-1]->fosse == '0'){

          do{
              printf("Quelle rangée voulez-vous réserver ? ");
              verif=scanf("%d",&rangee);
              viderBuffer();
          }while(verif!=1 || rangee<=0 || rangee > (s[num-1]->ntotligne));
          
          printf("\n");
          
          do{
               printf("Quelle colonne voulez-vous réserver ? ");
               verif=scanf("%d",&colonne);
               viderBuffer();
          }while(verif!=1 || colonne<=0 || colonne > (s[num-1]->ntotcolonne));
          
          printf("\n");
          
          do{
              printf("Avez-vous bien choisi la place de la %dème rangée et de la %dème colonne (répondre : 1 pour oui et 0 pour non) : ",rangee,colonne);
              verif=(scanf("%d",&verifchoix));
              viderBuffer();
          }while(verif!=1 || (verifchoix!=1 && verifchoix!=0));
        
          printf("\n");     
       
          if(verifchoix==0){
             printf("Vous avez donc mal choisi votre place\n\n");
             AfficheSalle(s[num-1]);
             choisirPlace(s,nombresalle);
          }
          else{
             if(s[num-1]->tabsiege[rangee-1][colonne-1]=='x'){
               printf("Siège déjà pris, veuillez changer\n\n");
               AfficheSalle(s[num-1]);
               choisirPlace(s,nombresalle);

             }
             else if(s[num-1]->tabsiege[rangee-1][colonne-1]=='o'){
                s[num-1]->tabsiege[rangee-1][colonne-1]='x';
                printf("Votre place numéro %d/%d a bien été réservé pour le concert de %s",rangee,colonne,s[num-1]->c.artiste);
                printf("\n");
                changerFichierSF(s[num-1]->nom,s[num-1]->tabsiege,s[num-1]->ntotligne,s[num-1]->ntotcolonne);
                AfficheSalle(s[num-1]);
                choisirPlace(s,nombresalle);
             } 
             else{
               printf("Place inexistante, veuillez changer\n\n");
               AfficheSalle(s[num-1]);
               choisirPlace(s,nombresalle);
             }  
          } 
        } 
        else if(s[num-1]->fosse=='1'){
            do{
               printf("Voulez-vous révervez une place en fosse ? (1 : oui et 0 : non) ");
               verif=scanf("%c",&choixfosse);
               viderBuffer(); 
            }while(verif!=1 || (choixfosse!='0' && choixfosse!='1'));
             
            printf("\n"); 
            
            if(s[num-1]->c.fosse <=0 && choixfosse=='1'){
                 printf("Il n'y a plus de place en fosse\n\n");
                 choixfosse='0';
            } 
             
            if(choixfosse=='1'){
                s[num-1]->c.fosse--;
                printf("Vous avez bien réservé une place en fosse pour le concert de %s",s[num-1]->c.artiste);
                printf("\n");
                changerFichierAF(s[num-1]->nom,s[num-1]->c.fosse,s[num-1]->ntotligne);
                printf("\n");
                AfficheSalle(s[num-1]);
                choisirPlace(s,nombresalle);
            }
            else{
                do{
                  printf("Quelle rangée voulez-vous réserver ? ");
                  verif=scanf("%d",&rangee);
                  viderBuffer();
                }while(verif!=1 || rangee<=s[num-1]->nligneA || rangee > (s[num-1]->ntotligne));
                
                printf("\n");
                
                do{
                 printf("Quelle colonne voulez-vous réserver ? ");
                 verif=scanf("%d",&colonne);
                 viderBuffer();
                }while(verif!=1 || colonne<=0 || colonne > (s[num-1]->ntotcolonne));
                
                printf("\n");
                  
                do{
                  printf("Avez-vous bien choisi la place de la %dème rangée et de la %dème colonne (répondre : 1 pour oui et 0 pour non) : ",rangee,colonne);
                  verif=(scanf("%d",&verifchoix));
                  viderBuffer();
                }while(verif!=1 || (verifchoix!=1 && verifchoix!=0));
        
                printf("\n");
                
                if(verifchoix==0){
                 printf("Vous avez donc mal choisi votre place\n\n");
                 AfficheSalle(s[num-1]);
                 choisirPlace(s,nombresalle);
                }
                else{
                   if(s[num-1]->tabsiege[rangee-1][colonne-1]=='x'){
                     printf("Siège déjà pris, veuillez changer\n\n");
                     AfficheSalle(s[num-1]);
                     choisirPlace(s,nombresalle);

                   }
                   else if(s[num-1]->tabsiege[rangee-1][colonne-1]=='o'){
                     s[num-1]->tabsiege[rangee-1][colonne-1]='x';
                     printf("Votre place numéro %d/%d a bien été réservé pour le concert de %s",rangee,colonne,s[num-1]->c.artiste);
                     changerFichierSF(s[num-1]->nom,s[num-1]->tabsiege,s[num-1]->ntotligne,s[num-1]->ntotcolonne);
                     printf("\n");
                     AfficheSalle(s[num-1]);
                     choisirPlace(s,nombresalle);
                   } 
                   else{
                   printf("Place inexistante, veuillez changer\n\n");
                   AfficheSalle(s[num-1]);
                   choisirPlace(s,nombresalle);
                   }
                }
          

            }
        }
    }  
    else{
      printf("\n-------\n\n");
    }     
}
 
 
 
void Festivalier(){ 
    int nombresalle=0;
    Salle** salles=NULL;
  
    printf("\nBienvenue dans le mode Festivalier !\n\n");
    
    sleep(1);
    
    printf("Voici les salles où il y a des concerts : \n\n");
    
    salles=recupSalle(&nombresalle);
    
    afficheConcertF(salles,nombresalle);
       
    sleep(1);
       
    choisirPlace(salles,nombresalle);
       
    
}  

 void ecrireNom(Salle**tabSalle,int taille){
    FILE*fichier=NULL;

    //Ouverture du fichier en mode ajout

    fichier=fopen("nomsalle.txt", "a");

    if (fichier == NULL){
      printf("Ouverture du fichier impossible\n\n");
      printf("Code d'erreur = %d \n\n", errno );
      printf("Message d'erreur = %s \n\n", strerror(errno) );
    }

    //Rajout du nom de la nouvelle salle

    else{
      for(int i=0;i<taille;i++){
        fprintf(fichier,"%s\n",(tabSalle[i]->nom));
      }
    }

    //Fermeture du fichier

    fclose(fichier);
  }

  void ecrireSalles(Salle* s) {
    char* f = malloc(sizeof(char) * 50);
    sprintf(f, "%s.txt", s->nom);
    FILE* fichier = NULL;

    //Ouverture du fichier en mode ajout

    fichier = fopen(f, "a");

    if (fichier == NULL) {
        printf("Ouverture du fichier impossible\n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno) );

    }
    else{

      //Fermeture du fichier

      fclose(fichier);

      //Ouverture du fichier en mode écriture

      fichier = fopen(f, "w");
      if (fichier == NULL) {
        printf("Ouverture du fichier impossible\n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno) );

      } 

      //Ecriture de toutes les données

      else {
        fprintf(fichier, "Nom de salle : %s\n", s->nom);
        fprintf(fichier, "Concert : %c\n", s->concert);
        fprintf(fichier, "Nombre de rangees : %d\n", s->ntotligne);
        fprintf(fichier, "Nombre de colonne max: %d\n", s->ntotcolonne);
        if(s->concert != '0') {
          fprintf(fichier, "Heure : %d/%d/%d %d:%d\n", s->c.heure.jour, s->c.heure.mois, s->c.heure.annee, s->c.heure.h, s->c.heure.min);
          fprintf(fichier, "Nom du concert : %s\n", s->c.artiste);
          fprintf(fichier, "Nombre de rangees A : %d\n", s->nligneA);
          fprintf(fichier, "Nombre de rangees B : %d\n", s->nligneB);
          fprintf(fichier, "Fosse : %c\n", s->fosse);
          fprintf(fichier, "Nombre fosse : %d\n", s->c.fosse);
          fprintf(fichier, "Prix rangee A : %d\n", s->c.prixA);
          fprintf(fichier, "Prix rangee B : %d\n", s->c.prixB);
          fprintf(fichier, "Prix rangee C : %d\n", s->c.prixC);
        } 
        else {
          fprintf(fichier, "Heure : 0/0/0 0:0\n");
          fprintf(fichier, "Nom du concert : 0\n");
          fprintf(fichier, "Nombre de rangees A : %d\n", s->nligneA);
          fprintf(fichier, "Nombre de rangees B : %d\n", s->nligneB);
          fprintf(fichier, "Fosse : %c\n", s->fosse);
          fprintf(fichier, "Nombre fosse : 0\n");
          fprintf(fichier, "Prix rangee A : 0\n");
          fprintf(fichier, "Prix rangee B : 0\n");
          fprintf(fichier, "Prix rangee C : 0\n");
        }
      }
      fprintf(fichier, "Plan : \n");
      
      for(int i=0;i<s->ntotligne;i++){
        for(int j=0;j<s->ntotcolonne;j++){
          if(s->tabsiege[i][j]=='o'){
            fputc(s->tabsiege[i][j],fichier);
          }
        }
        fputc('\n',fichier);
      }
    }
    free(f);
    fclose(fichier);
  }

  //Fonction qui créée une date et qui la retourne

  Date creerDate(){
    Date d;
    int a;
    do{
      printf("Entrez le jour: ");
      a=scanf("%d",&(d.jour));
      viderBuffer();
    }while(d.jour<=0 || d.jour>31 || a!=1); //Par convention,il n'y a que 30  jour dans un mois

    printf("\n");

    do{
      printf("Entrez le mois: ");
      a=scanf("%d",&(d.mois));
      viderBuffer();
    }while(d.mois<=0 || d.mois>12 || a!=1);

    printf("\n");

    do{
      printf("Entrez l'annee: ");
      a=scanf("%d",&(d.annee));
      viderBuffer();
    }while(d.annee<=2023|| a!=1);

    printf("\n");

    do{
      printf("Entrez l'heure: ");
      a=scanf("%d",&(d.h));
      viderBuffer();
    }while(d.h<0 || d.h>23 || a!=1);

    printf("\n");

    do{
      printf("Entrez les minutes: ");
      a=scanf("%d",&(d.min));
      viderBuffer();
    }while(d.min<0 || d.min>30 || a!=1);

    printf("\n");

    return d;
  }


  //Création du concert

  Concert creerConcert(){
    int a,b,d,e,f;
    Concert ct;
    do{
      printf("Entrez le nom de l'artiste (Le Nom et Prénom doivent être collés sinon l'artiste sera défini que par le premier mot indiqué): ");
      a=scanf("%49s",ct.artiste);
      viderBuffer();
    }while(a!=1 || strlen(ct.artiste)>20);

    printf("\n");

    do{
      printf("Prix de la catégorie A: ");
      d=scanf("%d",&(ct.prixA));
      viderBuffer();
    }while(d!=1 || ct.prixA<=0);

    printf("\n");

    do{
      printf("Prix de la catégorie B: ");
      e=scanf("%d",&(ct.prixB));
      viderBuffer();
    }while(e!=1 || ct.prixB<=0);

    printf("\n");

    do{
      printf("Prix de la catégorie C: ");
      f=scanf("%d",&(ct.prixC));
      viderBuffer();
    }while(f!=1 || ct.prixC<=0);

    printf("\n");

    ct.heure=creerDate();
    return ct;
  }

  //Fonction qui initialise et retourne une salle: elle retourne un pointeur sur une salle; (seulement a utiliser pour la fonction creerTabSalle() )
  void creerSalle(){
    int e;
    Salle *s1=malloc(sizeof(Salle));
    s1->concert = '0';
    s1->fosse = '0';
    do{
      printf("Entrer le nom de la salle (Le nom de la salle doit être en un seul morceau sinon ce sera que le premier mot qui sera gardé): ");
      e=scanf("%49s",s1->nom);
      viderBuffer();
    }while(e!=1 || strlen(s1->nom) > 20); 

    printf("\n");

    int a,b,c,d;
    do{
      printf("(Si le nombre de rangée > 20 alors toutes les rangées auront le même nombre de sièges et max = 99\n\nEntrer le nombre total de rangées : ");
      a=scanf("%d",&s1->ntotligne);
      viderBuffer();
    }while(s1->ntotligne<=1|| s1->ntotligne>=100 || a!=1);
    
    printf("\n");

    do{
      printf("(Max Siège par colonne = 99)\n\nEntrer le nombre total de siège par rangée: ");
      b=scanf("%d",&(s1->ntotcolonne));
      viderBuffer();
    }while(s1->ntotcolonne<=0|| s1->ntotcolonne>=100||b!=1);
    
    printf("\n");

    do{
      printf("Entrer le nombre total de rangéee A: ");
      c=scanf("%d",&(s1->nligneA));
      viderBuffer();
    }while(s1->nligneA<=0 || s1->nligneA>s1->ntotligne || c!=1);
    
    printf("\n");

    do{
      printf("Entrer le nombre total de rangée B: ");
      d=scanf("%d",&(s1->nligneB));
      viderBuffer();
    }while(s1->nligneB<0 || s1->nligneB>s1->ntotligne-s1->nligneA ||d!=1);
    
    printf("\n");

    //Faire deux cas différents, quand le nombre de colonnes est identique ou non

    if(s1->ntotligne<20){
      s1->tabsiege=malloc((s1->ntotligne+1)*sizeof(char*));

      if(s1->tabsiege==NULL){
        printf("\n12 - Erreur d'allocation \n");
      }
      else{
        for(int i=0;i<s1->ntotligne;i++){
          
          int nb;
            do{
               printf("Entrer le nombre de siège du rang %d: ",i+1);
               a=scanf("%d",&nb);
               viderBuffer();
            }while(nb<=0 || nb>s1->ntotcolonne || a!=1);

            printf("\n");

            s1->tabsiege[i]=malloc((nb)+1*sizeof(char));

            if(s1->tabsiege[i]==NULL){
              printf("13 - Erreur d'allocation");
            }

            else{
              for(int j=0;j<nb;j++){
                s1->tabsiege[i][j]='o';
              }
              s1->tabsiege[i][nb]='\0';
            }

             
          
          
        }
      }
    }
    else{
      s1->tabsiege=malloc((s1->ntotligne+1)*sizeof(char*));

      if(s1->tabsiege==NULL){
        printf("14 - Erreur d'allocation");
      }

      else{
        for(int i=0;i<s1->ntotligne;i++){
          s1->tabsiege[i]=malloc((s1->ntotcolonne)*sizeof(char));

          if(s1->tabsiege[i]==NULL){
            printf("15 - Erreur d'allocation");
          }

          else{
            for(int j=0;j<s1->ntotcolonne;j++){
              s1->tabsiege[i][j]='o';
            }
            s1->tabsiege[i][s1->ntotcolonne]='\0';
          } 
        }
      }
    }

    
    ecrireNom(&s1,1);
    ecrireSalles(s1);

  }

  //Création de plusieurs salles

  void creerPlusieursSalles(){
    int a,nb;
    do{
      printf("Combien de salles voulez vous créer (max 10): ");
      a=scanf("%d",&nb);
      viderBuffer();
    }while(nb<0 || nb>10 || a!=1);

    printf("\n");
    
    if(nb==0){
      return;
    }

    for(int i=0;i<nb;i++){
      creerSalle();
    }
  }

  int choixFosse(){
      int a,choix;
      do{
          printf("\n 1 - Fosse\n\n  2 - Sans fosse\n\nVotre choix: ");
          a=scanf("%d",&choix);
          viderBuffer();
      }while(a!=1 || choix<1 || choix>2);
      return choix;
  }

  //Créé une nouvelle salle et lui attribue un concert()
  void attribuer2C(){
    int choix;
    int nbr = 0;
    FILE* nomsalle = NULL;
    char recup[200];
    int a=-1;
    int verif;

    creerSalle();
    Salle*s=malloc(sizeof(Salle));
    if(s==NULL){
      printf("Erreur\n");
      free(s);
    }
    else{
      char*nom=retourneStr();

      //Vérification que le nom donné est bien celui d'une salle existante

      nomsalle=fopen("nomsalle.txt","r");

      if(nomsalle==NULL){
        printf("Erreur d'ouverture du fichier des noms de salles\n\n");
        return;
      }

      nbr=recupNombreSalle(nomsalle);

      for(int i=0;i<=nbr;i++){
        fscanf(nomsalle,"%s\n",recup);
        a=strcmp(recup,nom);
        if(a==0){
          
          verif=1;
        }
      }
      fclose(nomsalle);

      if(verif!=1){
        printf("Le nom de scène n'existe pas veuillez recommencer\n\n");
        return;
      }
      for(int i=0;i<200;i++){
        if(nom[i]!='\n'){
          recup[i]=nom[i];
        }
        else{
          i=201;
        }
      }

      s=recupInfoM(recup);
      if(s==NULL){
        printf("16 - Erreur d'allocation\n");
      }
      else{
        for(int i=0;i<strlen(s->nom);i++){
          if(s->nom[i]==' ' || (s->nom[i]=='\n')){
            s->nom[i]='\0';
          }
        }
        s->c=creerConcert();

        s->concert='1';

        choix=choixFosse();

        if(choix){
          s->fosse='1';
          s->c.fosse=0;
          int j=0;
          for(int i=0;i<s->nligneA;i++){
            while(s->tabsiege[i][j]=='o' || s->tabsiege[i][j]=='x'){
              s->c.fosse++;
              j++;  
            }
          }
          s->c.fosse*=2;
        }
        else{
          s->fosse='0';
          s->c.fosse=0;
        }
      }
      s->ntotcolonne--;

      ecrireSalles(s);
      free(s);
    }
  }

  //attribue a une salle deja existante
  void attribuer1C(Salle*s){
  int a,f;

    if(s->concert=='0'){
      s->concert='1';
      s->c=creerConcert();
      
      do{
        printf("Fosse :\n 1-OUI\n  0-NON\n\nVotre choix:");
        a=scanf("%d",&f);
        viderBuffer();
      }while(a!=1||f<0 || f>1);

      if(f==1){
        s->fosse='1';
        s->c.fosse=0;
        int j=0;

        for(int i=0;i<s->nligneA;i++){
          while(s->tabsiege[i][j]=='o' || s->tabsiege[i][j]=='x'){
            s->c.fosse++;
            j++;  
          }
        }
        s->c.fosse*=2;
      }

      else{
        s->c.fosse=0;
        s->fosse='0';
        int j=0;
        for(int i=0;i<s->nligneA;i++){
          while(s->tabsiege[i][j]=='o' || s->tabsiege[i][j]=='x'){
            s->c.fosse++;
            j++;  
          }
        }
        s->c.fosse*=2;
      }

      for(int i=0;i<s->ntotligne;i++){
        for(int j=0;j<strlen(s->tabsiege[i]);j++){
          if(s->tabsiege[i][j]=='x'){
            s->tabsiege[i][j]='x'; 
          }
          else if(s->tabsiege[i][j]=='o'){
            s->tabsiege[i][j]='o';
          }
          else{
            s->tabsiege[i][j]='\0';
          }
        }
      }

      ecrireSalles(s);
    }
    else{
      printf("Il y a deja un concert dans cette salle\n\n");
    }
  }

  int choixAttribuerC(){
    int a,choix;

    do{
      printf("Vous allez attribuer un concert: \n\n 1 - Salle existante\n\n  2 - Nouvelle Salle\n\nVotre Choix: ");
      a=scanf("%d",&choix);
      viderBuffer();
    }while(a!=1 || choix<1 || choix>2);

    printf("\n");
    return choix;
  }


  void attribuerC(){
    int a;
    int choix=choixAttribuerC();
    FILE* nomsalle = NULL;
    int verif = 0;
    int nbr = -1;
    char recup[200];

    if(choix==2){
      printf("vous avez choisi de créer une nouvelle salle\n\n");
      attribuer2C();
    }
    else{
      Salle*s=malloc(sizeof(Salle));
      char*nom=retourneStr();

      //Vérification que le nom donné est bien celui d'une salle existante

      nomsalle=fopen("nomsalle.txt","r");

      if(nomsalle==NULL){
        printf("Erreur d'ouverture du fichier des noms de salles\n\n");
        return;
      }

      nbr=recupNombreSalle(nomsalle);

      for(int i=0;i<=nbr;i++){
        fscanf(nomsalle,"%s\n",recup);
        a=strcmp(recup,nom);
        if(a==0){
          
          verif=1;
        }
      }
      fclose(nomsalle);

      if(verif!=1){
        printf("Le nom de scène n'existe pas veuillez recommencer\n\n");
        return;
      }
      for(int i=0;i<200;i++){
        if(nom[i]!='\n'){
          recup[i]=nom[i];
        }
        else{
          i=201;
        }
      }

      s=recupInfoM(nom);
      if(s==NULL){
        return;
      }  
      else{
        for(int i=0;i<strlen(s->nom);i++){
          if(s->nom[i]==' ' || (s->nom[i]=='\n')){
            s->nom[i]='\0';
          }
        }
        s->ntotcolonne--;
        attribuer1C(s);
      }
      free(s); 
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


  int nvConcert(){
    int a,choix;

    do{
      printf("Que voulez vous faire de cette salle: ");
      printf("\n\n1-Supprimer la salle\n\n ");
      printf("2-Changer les catégorie\n\n  ");
      printf("3-Retour\n\nVotre Choix: ");
      a=scanf("%d",&choix);
      viderBuffer();
    }while(a!=1||choix<1||choix>3);

    printf("\n");

    return choix;
  }

  //Soit Salle *s, un pointeur, cette fonction change les valeurs des rang A et B de la salle s

  void changeCategorie(Salle*s){
    int a;

    do{
      printf("Entrez le nombre de rang de catégorie A: ");
      a=scanf("%d",&((s)->nligneA));
    }while(a!=1||s->nligneA<0 || (s)->nligneA>(s)->ntotligne);

    do{
      printf("Entrez le nombre de rang de catégorie B: ");
      a=scanf("%d",&(s->nligneB));
    }while(a!=1|| s->nligneB>s->ntotligne-s->nligneA || s->nligneB<0);

  }


  // fin d'un concert + attribuer un nv concert:
  void suppC(Salle*s){
    int choix=nvConcert();

    while(1){
      if(s->concert=='1'){
        Date dateloc=HeureLocale();

        if(verifDate(dateloc,s->c.heure)==-1){   /*compare la date de fin de concert avec la date actuelle*/
          s->concert='0';

          for(int i=0;i<s->ntotligne;i++){

            for(int j=0;j<strlen(s->tabsiege[i]);j++){

              if(s->tabsiege[i][j]=='x'){
                s->tabsiege[i][j]='o'; 
              }

              if(s->tabsiege[i][j]=='\n'){
                s->tabsiege[i][j]='\0';
              }
            }
          }
          printf("\n\nVous avez supprimer ce concert: Aucun concert dans cette salle\n\n");
          ecrireSalles(s);

          int choix=nvConcert();

          if(choix==1){
            printf("\nVous avez choisi d'attribuer un nouveau concert a la salle %s\n\n",s->nom);
            s->c=creerConcert();
            s->concert='1';
            ecrireSalles(s);
          }

          else if(choix==2){
            changeCategorie(s);
            ecrireSalles(s);
          }

          else{
            printf("\nVous avez choisi retour\n\n");
            break;  
          }
        }

        else {
          printf("\n\nConcert non terminer\n\n");
          break;
        }
      }

      else{
        printf("\n\nAucun concert dans cette salle\n\n");
        break;
      }
    }

  }

  /* calcule le ratio sièges réservés/nombre total de sièges lorsqu'il n'y a pas de fosse*/
  float ratioSF(Salle*s1){
    float result=0;
    float nbPlaceTot = 0;
    int j=0;

    for(int i=0;i<s1->ntotligne;i++){

      while(s1->tabsiege[i][j]=='o'||s1->tabsiege[i][j]=='x'){
        nbPlaceTot++;

        if(s1->tabsiege[i][j]=='x'){
          result+=1;
        }
        j++;
      }
    }
    return result/nbPlaceTot;
  }


  /* calcule le ratio sièges réservés/nombre total de sièges lorsqu'il 'y a une fosse*/
  float ratioAF(Salle*s1){
    float result=0;
    float nbPlaceTot =0;
    int j=0;

    for(int i=0;i<s1->nligneA;i++){

      while(s1->tabsiege[i][j]=='o'||s1->tabsiege[i][j]=='x'){
        nbPlaceTot++;  
        j++;
      }
    }

    nbPlaceTot*=2;
    j=0;

    for(int i=s1->nligneA;i<s1->ntotligne;i++){

      while(s1->tabsiege[i][j]=='o'||s1->tabsiege[i][j]=='x'){
        nbPlaceTot++;  
        j++;
      }
    }

    j=0;

    result=nbPlaceTot-s1->c.fosse;

    for(int i=s1->nligneA;i<s1->ntotligne;i++){

      while(s1->tabsiege[i][j]=='o'||s1->tabsiege[i][j]=='x'){
        if(s1->tabsiege[i][j]=='x'){
          result+=1;
        }
        j++;
      }
    }

    return result/nbPlaceTot;
  }

  //Fonction générale du ratio

  void ratio(Salle*s){
    if(s->concert=='1' && s->fosse=='1'){
      printf("Avec fosse: le ratio de la salle %s est : %f\n\n",s->nom,ratioAF(s));
    }
    else if(s->concert=='1' && s->fosse=='0'){
     printf("Sans fosse: le ratio de la salle %s est : %f\n\n",s->nom,ratioSF(s));
    }
    else{
      printf("il n'y a pas de concert\n\n");
    }
  }

  //Fonction qui récupère le choix du manager

  int modeM_choix(){
    int choix,a;

    do{

      afficheNomsalle();

      printf("\n\nBienvenue dans le Mode manager: \n\n\n");
      printf("1 - Creer une/plusieurs salle de concert\n\n ");
      printf("2 - Attribuer un concert dans une salle\n\n  ");
      printf("3 - Modifier la configuration des salles dont les concerts sont fini\n\n   ");
      printf("4 - Observer une salle\n\n    ");
      printf("5 - consulter le ratio sièges réservés / nombre total de sièges.\n\n     ");
      printf("6 - Retour\n\n");
      printf("Votre choix : ");
      a=scanf("%d",&choix);
      viderBuffer();
      printf("\n-------\n\n");
    }while(choix<1 || choix>6 || a!=1);


    return choix;
  }



  void Manager(){
    int choix;
    FILE* nomsalle=NULL;
    int verif=-1;
    char recup[200];
    int nbr = 0;
    int a = -1;

    //Faire un boucle pour ne pas sortir du programme tant que l'utilisateur ne l'aura pas dit

    while(1){
      Salle *s=malloc(sizeof(Salle));
      choix=modeM_choix();

      if(choix==1){
        creerPlusieursSalles();
        free(s);
      }
      else if(choix==2){
        attribuerC();
        free(s); 
      }
      else if(choix==3){
        printf("\n\nLa salle dont vous voulez modifier la configuration :\n\n");
        char*nom=retourneStr();

        //Vérification que le nom donné est bien celui d'une salle existante
        
        nomsalle=fopen("nomsalle.txt","r");

        if(nomsalle==NULL){
        printf("Erreur d'ouverture du fichier des noms de salles\n\n");
        return;
        }

        nbr=recupNombreSalle(nomsalle);

        for(int i=0;i<=nbr;i++){
          fscanf(nomsalle,"%s\n",recup);
          a=strcmp(recup,nom);

          if(a==0){
            verif=1;
          }
        }
        fclose(nomsalle);

        if(verif!=1){
          printf("Le nom de scène n'existe pas veuillez recommencer\n\n");
          Manager();
        }

        s=recupInfoM(nom);
        if(verifDate(HeureLocale(),s->c.heure)==1){
          printf("\nIl y a déjà un concert de prévu, vous ne pouvez pas modifier la configuration de la salle\n\n");
        }
        else{
          if(s==NULL){
          printf("18 - Erreur d'allocation\n");
          }
          else{
            for(int i=0;i<strlen(s->nom);i++){
              if(s->nom[i]==' ' || (s->nom[i]=='\n')){
                s->nom[i]='\0';
              }
            }
            s->ntotcolonne--;
            suppC(s);
          }
        }
        

        free(s);
      }
      else if(choix==4){
        printf("\n\nLa salle que vous voulez observer : \n");

        char*nom=retourneStr();

        //Vérification que le nom donné est bien celui d'une salle existante

        nomsalle=fopen("nomsalle.txt","r");

        if(nomsalle==NULL){
        printf("Erreur d'ouverture du fichier des noms de salles\n\n");
        return;
        }

        nbr=recupNombreSalle(nomsalle);

        for(int i=0;i<=nbr;i++){
          fscanf(nomsalle,"%s\n",recup);
          a=strcmp(recup,nom);

          if(a==0){
            verif=1;
          }
        }
        fclose(nomsalle);

        if(verif!=1){
          printf("Le nom de scène n'existe pas veuillez recommencer\n\n");
          Manager();
        }

        s=recupInfoM(nom);
          if(s==NULL){
            printf("19 - Erreur d'allocation\n\n");
          }
          else{
            for(int i=0;i<strlen(s->nom);i++){
              if(s->nom[i]==' ' || (s->nom[i]=='\n')){
                s->nom[i]='\0';
              }
            }

            sleep(1);

            AfficheSalleM(s);

            sleep(1);
          }
        free(s);
        free(nom);
      } 
      else if(choix==5){
         printf("\n\nRatio de la salle:\n\n");

        char*nom=retourneStr();

        //Vérification que le nom donné est bien celui d'une salle existante

        nomsalle=fopen("nomsalle.txt","r");

        if(nomsalle==NULL){
        printf("Erreur d'ouverture du fichier des noms de salles\n\n");
        return;
        }

        nbr=recupNombreSalle(nomsalle);

        for(int i=0;i<=nbr;i++){
          fscanf(nomsalle,"%s\n",recup);
          a=strcmp(recup,nom);

          if(a==0){
            verif=1;
          }
        }
        fclose(nomsalle);

        if(verif!=1){
          printf("Le nom de scène n'existe pas veuillez recommencer\n\n");
          Manager();
        }

        s=recupInfoM(nom);
        if(s==NULL){
          return;
        }
        else{
          for(int i=0;i<strlen(s->nom);i++){
            if(s->nom[i]==' ' || (s->nom[i]=='\n')){
              s->nom[i]='\0';
            }
          }
          ratio(s);
        }
        free(s);
      }
      else{
        free(s);
        break;
      }
    }   
  }

//Choix du mode

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

  int main() {

      printf("              BIENVENUE");
      printf("\n\n\n              CY-FEST\n");
      printf("________________________________________\n\n");

      sleep(1);

      int choix;

      //Boucle pour ne pas sortir du progamme tant que cela n'a pas été indiqué

      while(1){
          choix=choixMode();
          if(choix==1){
              printf("\n----------------\n");
              Manager();
          }
          else if(choix==2){
              printf("\n----------------\n");
              Festivalier();
          }
          else{
              break;
          }
      }
    return 0;
  }
