#include "header1.h"
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
