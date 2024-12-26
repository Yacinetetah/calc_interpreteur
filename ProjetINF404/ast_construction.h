
#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast op_g , Ast op_d) ;
// renvoie un arbre abstrait de nature OPERATEUR,  d'operateur opr 
// et ayant opde_gauche et opde_droit pour fils gauche et droit

Ast creer_valeur(int val) ; 
// renvoie un arbre abstrait "feuille", de nature OPERATION  
// et de valeur val

Ast creer_sepinst(Ast A1, Ast A2);

Ast creer_identif(char *chaine);

Ast creer_aff(Ast ArbG, Ast ArbD);

Ast creer_lire(Ast ArbG);

Ast creer_ecrire(Ast ArbG);

Ast creer_Si(Ast ArbSi, Ast ArbAlors, Ast ArbSinon);

Ast creer_TQ(Ast ArbSi, Ast ArbBody);

Ast creer_comp(TypeCompar opcomp, Ast op_g , Ast op_d);

#endif
 