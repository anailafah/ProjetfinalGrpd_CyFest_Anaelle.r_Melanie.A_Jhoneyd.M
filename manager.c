#include "manager.h"

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
  }while(s1->ntotligne<=1|| s1->ntotligne=>100 || a!=1);

  printf("\n");

  do{
    printf("(Max Siège par colonne = 99)\n\nEntrer le nombre total de siège par rangée: ");
    b=scanf("%d",&(s1->ntotcolonne));
    viderBuffer();
  }while(s1->ntotcolonne<=0|| s1->ntotcolonne=>100||b!=1);

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
