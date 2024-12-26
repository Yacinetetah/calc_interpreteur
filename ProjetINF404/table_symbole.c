#include "table_symbole.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void initTS(TAB_IDF *tab_idf) {
    tab_idf->NbSymb = 0;
    return;
}


void afficheTS(TAB_IDF *tab_idf){
    if (tab_idf->NbSymb == 0){
        printf("\nIl n'y a pas d'identificateurs dans la table\n");
        return;
    }
    printf("\nTable identificateurs: ");
    for(int i = 0; i<tab_idf->NbSymb; i++){
        printf("\n%s = %d\n",tab_idf->TS[i].nom, tab_idf->TS[i].valeur);
    }
    return;
}

int estPresentTS(TAB_IDF *tab_idf, char *idf, int *v) {
    for(int i=0; i<tab_idf->NbSymb; i++) {
        if (!strcmp(tab_idf->TS[i].nom, idf)) {
            *v = tab_idf->TS[i].valeur;
            return 1;
        }
    }
    printf("\nIdentificateur NOT FOUND\n");
    exit(0);
}

void insererTS(TAB_IDF *tab_idf, char *idf, int v) {
    for(int i=0; i<tab_idf->NbSymb; i++) {
        if (!strcmp(tab_idf->TS[i].nom, idf)) {
            tab_idf->TS[i].valeur = v;
            return;
        }
    }
    strcpy(tab_idf->TS[tab_idf->NbSymb].nom, idf);
    tab_idf->TS[tab_idf->NbSymb].valeur = v;
    tab_idf->NbSymb += 1;
    return;
}
 
void modifierTS(TAB_IDF *tab_idf, char *idf, int v) {
    for(int i=0; i<tab_idf->NbSymb; i++) {
        if (!strcmp(tab_idf->TS[i].nom, idf)) {
            tab_idf->TS[i].valeur = v;
            return;
        }
    }
    printf("\nIdentificateur NOT FOUND\n");
    exit(0);
}

