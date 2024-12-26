
/* Exemple d'utilisation de l'analyse lexicale : 
    Affichage d'une sequence de lexemes            */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "ast_parcours.h"
#include "table_symbole.h"

int main (int argc, char *argv[]) {
   if (argc>2){
        printf("nombre d'arguments incorrects ! il faut donner un fichier.\n usage: ./calculette <nom_fichier>\n");
        exit(1) ;
   } ;
   TAB_IDF table;
   initTS(&table);
   
   Ast A = NULL;
   
   // demarre l'analyse sur le fichier transmis en argument
   analyser(argv[1], &A, &table);
   
   // affichage de l'arbre de l'expression
   printf("Arbre abstrait de l'expression\n");
   afficher2(A);
   //calcul et interpretation du resultat
   printf("\nInterpretation de l'arbre\n");
   evaluer(A, &table);
   
   //table des identificateurs à la fin de l'interpretation
   
   afficheTS(&table);
   //arreter() ; // termine l'analyse lexicale
   return 0 ;
}
 
 