
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {OPERATION, VALEUR, INSTRUCTION, IDF2, COMPR} TypeAst ;
typedef enum {N_PLUS, N_MUL, N_MOINS, N_DIV} TypeOperateur ;
typedef enum {N_EQUAL, N_NEQUAL, N_GREATER, N_LESS, N_SUPEQUAL, N_INFEQUAL} TypeCompar;
typedef enum {N_SEPINST, N_AFF, N_LIRE, N_ECRIRE, N_SI, N_TANTQUE} TypeInstruction;

typedef struct noeud { 
	TypeAst nature ;
	TypeOperateur operateur ;
  TypeInstruction instruction ;
  TypeCompar op_comp;
	struct noeud *gauche, *middle, *droite ;
  int valeur ;
  char chaine[256];
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
  