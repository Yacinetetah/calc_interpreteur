#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__

#include "table_symbole.h"

void afficher2(Ast expr) ;
// affiche l'arbre abstrait de l'expression arithmetique expr

int evaluation(Ast expr, TAB_IDF *table) ;
// calcule la valeur de l'expression arithmetique expr
// FONCTION A COMPLETER !
void aff_operateur(TypeOperateur op);

int valeur_b(Ast expr, TAB_IDF *table);

void evaluer_affec(Ast expr, TAB_IDF *table);

void evaluer_lr(Ast expr, TAB_IDF *table); 

void evaluer_er(Ast expr, TAB_IDF *table);

void evaluerSi(Ast expr, TAB_IDF *table);

void evaluerTQ(Ast expr, TAB_IDF *table);

void evaluer(Ast expr, TAB_IDF *table);

#endif

 