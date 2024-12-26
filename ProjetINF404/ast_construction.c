#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 
#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast op_g , Ast op_d) 
{
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = OPERATION;
      expr->operateur = opr;
      if (op_g == NULL || op_d == NULL) {
         printf("ERREUR_EXPRESSION\n") ;
	 exit(1) ;
      } else {
         expr->gauche = op_g;
         expr->droite = op_d;
      } ;
      return expr ;
} 

Ast creer_valeur(int val) { 
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = VALEUR;
      expr->valeur = val;
      return expr ;
}
 
Ast creer_sepinst(Ast A1, Ast A2)
{
    Ast inst = (Ast)malloc(sizeof(NoeudAst));
    inst->nature = INSTRUCTION;
    inst->instruction = N_SEPINST;
    inst->gauche = A1;
    inst->droite = A2;
    return inst;
}

Ast creer_identif(char *chaine)
{
    Ast idf = (Ast)malloc(sizeof(NoeudAst));
    idf->nature = IDF2;
    strcpy(idf->chaine, chaine);
    return idf;
}

Ast creer_aff(Ast ArbG, Ast ArbD)
{
    Ast aff = (Ast)malloc(sizeof(NoeudAst));
    aff->nature = INSTRUCTION;
    aff->instruction = N_AFF;
    aff->gauche = ArbG;
    aff->droite = ArbD;
    return aff;
}

Ast creer_lire(Ast ArbG)
{
    Ast lire = (Ast)malloc(sizeof(NoeudAst));
    lire->nature = INSTRUCTION;
    lire->instruction = N_LIRE;
    lire->gauche = ArbG;
    return lire;
}

Ast creer_ecrire(Ast ArbG)
{
    Ast ecrire = (Ast)malloc(sizeof(NoeudAst));
    ecrire->nature = INSTRUCTION;
    ecrire->instruction = N_ECRIRE;
    ecrire->gauche = ArbG;
    return ecrire;
}

Ast creer_Si(Ast ArbSi, Ast ArbAlors, Ast ArbSinon){
    Ast cd = (Ast)malloc(sizeof(NoeudAst));
    cd->nature = INSTRUCTION;
    cd->instruction = N_SI;
    cd->gauche = ArbSi;
    cd->middle = ArbAlors;
    cd->droite = ArbSinon;    
    return cd;
}

Ast creer_TQ(Ast ArbSi, Ast ArbBody){
    Ast tq = (Ast)malloc(sizeof(NoeudAst));
    tq->nature = INSTRUCTION;
    tq->instruction = N_TANTQUE;
    tq->gauche = ArbSi;
    tq->droite = ArbBody;
    return tq;
}

Ast creer_comp(TypeCompar opcomp, Ast op_g , Ast op_d)
{
    Ast Arbre;
    Arbre= (Ast) malloc(sizeof(NoeudAst));
    Arbre->nature = COMPR;
    Arbre->op_comp = opcomp;
    if (op_g == NULL || op_d == NULL) {
         printf("ERREUR EXPRESSION COMPARAISON\n") ;
	       exit(1) ;
      } else {
         Arbre->gauche = op_g;
         Arbre->droite = op_d;
      } ;
      return Arbre ;
}