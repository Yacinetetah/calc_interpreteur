#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type_ast.h"
#include "table_symbole.h"
#include "ast_parcours.h"





/*Ce bloc de code implémente des fonctions pour évaluer et afficher un arbre syntaxique abstrait (AST) généré lors de l'analyse syntaxique
 d'un programme. La fonction evaluer prend en charge l'évaluation des instructions de l'AST, telles que l'affectation, la lecture, 
 l'écriture, les structures de contrôle (si et tant que), ainsi que les séquences d'instructions. Chaque instruction est évaluée en 
 appelant des fonctions spécifiques. La fonction afficher2 est utilisée pour afficher l'AST, en parcourant récursivement les nœuds de 
 l'arbre et en affichant leurs valeurs et opérateurs correspondants. Les fonctions evaluation et valeur_b sont utilisées pour évaluer 
 les expressions arithmétiques et les expressions booléennes respectivement. Les fonctions evaluer_affec, evaluerTQ, evaluer_lr, 
 evaluer_er et evaluerSi sont des sous-fonctions de evaluer, chacune étant responsable de l'évaluation d'un type spécifique d'instruction. Enfin, les fonctions aff_operateur, rec_op1 et rec_op2 sont utilisées pour l'affichage des opérateurs lors de l'affichage de l'AST.*/


void evaluer(Ast expr, TAB_IDF *table)
{
    if (!expr) return;
    switch (expr->nature)
    {
        case INSTRUCTION:
            switch (expr->instruction)
            {
                case N_SEPINST:
                    evaluer(expr->gauche, table);
                    evaluer(expr->droite, table);
                    break;
            
                case N_AFF:
                    evaluer_affec(expr, table);
                    break;
                
                case N_LIRE:
                    evaluer_lr(expr, table);
                    break;
                    
                case N_ECRIRE:
                    evaluer_er(expr, table);
                    break;
                    
                case N_SI:
                    evaluerSi(expr, table);
                    break;
                
                case N_TANTQUE:
                    evaluerTQ(expr, table);
                    break;
            }
            break;
            
        default:
            printf("ERREUR: pas d'instruction donnée \n");
            exit(0);
    }
}


void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf("+") ;
			break;
		case N_MOINS:
			printf("-") ;
			break;
		case N_MUL:
			printf("*") ;
			break;
		case N_DIV:
			printf("/") ;
			break;
	}  
}
//Affichage de l'arbre
void afficher2(Ast expr) {
  if (!expr) { return; }
	switch (expr->nature) {
         case OPERATION:
                afficher2(expr->gauche);
                aff_operateur(expr->operateur) ;
                afficher2(expr->droite);
                break ;
         case VALEUR:
           	    printf("%d", expr->valeur);
           	    break ;
         case IDF2:
                printf("%s", expr->chaine);
                break;
            
         case INSTRUCTION:
            switch (expr->instruction)
            {
                case N_AFF:
                    afficher2(expr->gauche);
                    printf("=");
                    afficher2(expr->droite);
                    break;
                
                case N_LIRE:
                    printf("lire(");
                    afficher2(expr->gauche);
                    printf(")");
                    break;
                
                case N_ECRIRE:
                    printf("ecrire(");
                    afficher2(expr->gauche);
                    printf(")");
                    break;
                
                case N_SEPINST:
                    afficher2(expr->gauche);
                    printf(";\n");
                    afficher2(expr->droite);
                    break;
                
                case N_SI:
                    printf("si ");
                    afficher2(expr->gauche);
                    printf("\nalors\n");
                    afficher2(expr->middle);
                    printf("sinon\n");
                    afficher2(expr->droite);
                    printf("finSi");
                    break;
                    
                case N_TANTQUE:
                    printf("tantque");
                    afficher2(expr->gauche);
                    printf("\nfaire\n");
                    afficher2(expr->droite);
                    printf("fintantque");
                    break;
            }
            break;
        
        case COMPR:
            afficher2(expr->gauche);
            switch (expr->op_comp)
            {
                case N_EQUAL:
                    printf(" == ");
                    break;
                    
                case N_NEQUAL:
                    printf(" != ");
                    break;
                    
                case N_GREATER:
                    printf(" > ");
                    break;
                    
                case N_LESS:
                    printf(" < ");
                    break;
                    
                case N_SUPEQUAL:
                    printf(" >= ");
                    break;
                    
                case N_INFEQUAL:
                    printf(" <= ");
                    break;
            }
            afficher2(expr->droite);
            break;
        
        default:
            printf("%d\n", expr->nature);
            printf("ERREUR NOEUD\n");
            exit(0);
    }
}

int evaluation(Ast expr, TAB_IDF *table) {
    int *v = (int *)malloc(sizeof(int));
    switch(expr->nature){
    case VALEUR:
			    return expr->valeur;
		case OPERATION:
			switch (expr->operateur){
					case N_MUL:
						return evaluation(expr->gauche, table)*evaluation(expr->droite, table);
					case N_MOINS:
						return evaluation(expr->gauche, table)-evaluation(expr->droite, table);
					case N_PLUS:
						return evaluation(expr->gauche, table)+evaluation(expr->droite, table);
					case N_DIV:
						return evaluation(expr->gauche, table)/evaluation(expr->droite, table);
					default:
						return -1;
			}
     case IDF2:
           estPresentTS(table, expr->chaine, v); 
           return *v;              
		default:
      		return -1;
	}
}

int valeur_b(Ast expr, TAB_IDF *table)
{
    int  v_g, v_d;
    
    if (expr->nature == COMPR)
    {
        v_g = evaluation(expr->gauche, table);
        v_d = evaluation(expr->droite, table);
        
        switch (expr->op_comp)
        {
            case N_EQUAL:
                return v_g == v_d;
                
            case N_NEQUAL:
                return v_g != v_d;
                
            case N_GREATER:
                return v_g > v_d;
                
            case N_LESS:
                return v_g < v_d;
                
            case N_SUPEQUAL:
                return v_g >= v_d;
                
            case N_INFEQUAL:
                return v_g <= v_d;
        }
    }
    
    return -1;
}

void evaluer_affec(Ast expr, TAB_IDF *table)
{
    char idf[64];
    int v;
    
    strcpy(idf, expr->gauche->chaine);
    
    v = evaluation(expr->droite, table);
    insererTS(table, idf, v);
}

void evaluerTQ(Ast expr, TAB_IDF *table)
{
    int c;
    c = valeur_b(expr->gauche, table);
    if (c)
    {
        evaluer(expr->droite, table);
        evaluerTQ(expr, table);
    }
}



void evaluer_lr(Ast expr, TAB_IDF *table)
{
    int v;
    
    printf("\nVeuillez entrer une valeur entière %s : ", expr->gauche->chaine);
    scanf("%d", &v);
    insererTS(table, expr->gauche->chaine, v);
}

void evaluer_er(Ast expr, TAB_IDF *table)
{
    int v;
    v = evaluation(expr->gauche, table);
    printf("\nécriture d'identificateur: %d\n", v);
}

void evaluerSi(Ast expr, TAB_IDF *table)
{
    int c;
    
    c = valeur_b(expr->gauche, table);
    if (c)
    {
        evaluer(expr->middle, table);
    }
    else
    {
        evaluer(expr->droite, table);
    }
}






