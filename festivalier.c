#include "festivalier.h"

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
  if(s->nligneA<0 || s->nligneB<0 || s->tabsiege==NULL  || s->c.prixA<0 || s->c.prixB<0 || s->c.prixC<0 || (s->concert!='0' && (s->concert!='1'))){
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
  free(salles);
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
      printf("9 - Erreur d'allocation");
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
