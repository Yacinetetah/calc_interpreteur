#ifndef ___TS___
#define ___TS___

#define LIDF 20 // nombre maximum de caractéres pour un IDF
#define NBMAXSYMB 100 // nombre maximum d'IDF

typedef struct {
    char nom[100] ; // nom d'un identificateur
    int valeur ; // valeur de cet identificateur
} Couple ;

typedef struct {
    Couple TS[NBMAXSYMB] ;
    int NbSymb ;
} TAB_IDF ;

void initTS(TAB_IDF *tab_idf) ;
// intialise la TS en effectant 0 à NbSymb

void afficheTS(TAB_IDF *tab_idf) ;
// affiche le contenu complet de la TS

int estPresentTS(TAB_IDF *tab_idf, char *idf, int *v) ;
// si un couple (idf, v') est present dans la table alors
// affecte v' à *v
// estPresentTS renvoie vrai
// sinon
// estPresentTS renvoie faux

void insererTS(TAB_IDF *tab_idf, char *idf, int v) ;
// si un couple (idf,v') est present dans la table alors
// remplace la valeur v' par v
// sinon
// ajoute le couple (idf,v) dans la TS

void modifierTS(TAB_IDF *tab_idf, char *idf, int v) ;

#endif 