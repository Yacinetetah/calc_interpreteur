/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"
#include "table_symbole.h" 




/*Les déclarations de fonctions présentées définissent les prototypes de différentes fonctions utilisées dans un analyseur syntaxique 
ou un compilateur pour un langage de programmation. Chaque fonction correspond à une étape spécifique du processus d'analyse syntaxique, 
telles que la reconnaissance des facteurs, des termes, des séquences d'instructions, des conditions, des opérateurs, etc. Chaque fonction
 prend en paramètre un lexème courant, une structure d'arbre syntaxique (Ast), et éventuellement une table de symboles (TAB_IDF), 
 nécessaire pour la gestion des identificateurs et de leurs propriétés. L'analyseur principal (fonction "analyser") 
 serait responsable de coordonner ces différentes étapes pour analyser un fichier source et construire l'arbre syntaxique correspondant.*/


void rec_facteur(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_suite_seq_terme(Lexeme lexeme_C, Ast Arb1, Ast *Arb2, TAB_IDF *table);
void rec_seq_terme(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_suite_seq_facteur(Lexeme lexeme_C, Ast Arb1, Ast *Arb2, TAB_IDF *table);
void rec_seq_facteur(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_terme(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_seq_inst(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_condition(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void analyser(char *fichier, Ast *Arb1, TAB_IDF *table);
void rec_eag(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_inst(Lexeme lexeme_C, Ast *Arb1, TAB_IDF *table);
void rec_suite_seq_inst(Lexeme lexeme_C, Ast Arb1, Ast *Arb2, TAB_IDF *table);
void rec_op2(Lexeme lexeme_C, TypeOperateur *OP);
void rec_op1(Lexeme lexeme_C, TypeOperateur *OP);

void rec_seq_terme(Lexeme LexC, Ast *Arbre1, TAB_IDF *table){
    Ast Arbre;
    LexC = lexeme_courant();
    rec_terme(LexC, &Arbre, table);
    rec_suite_seq_terme(LexC, Arbre, Arbre1, table);
}

void rec_suite_seq_terme(Lexeme LexC, Ast Arbre1, Ast *Arbre2, TAB_IDF *table){
    Ast Arbre3, Arbre4;
    TypeOperateur OP;
    LexC = lexeme_courant();
    switch(LexC.nature){
        case PLUS:
        case MOINS:
            rec_op1(LexC, &OP);
            LexC = lexeme_courant();
            rec_terme(LexC, &Arbre3, table);
            Arbre4 = creer_operation(OP, Arbre1, Arbre3);
            rec_suite_seq_terme(LexC, Arbre4, Arbre2, table);
            break;
        
        default:
            *Arbre2 = Arbre1;
            break;
    }
}



void rec_seq_facteur(Lexeme LexC, Ast *Arbre1, TAB_IDF *table){
    Ast Arbre;
    LexC = lexeme_courant();
    rec_facteur(LexC, &Arbre, table);
    rec_suite_seq_facteur(LexC, Arbre, Arbre1, table);
}

void rec_terme(Lexeme LexC, Ast *Arbre1, TAB_IDF *table){
    LexC = lexeme_courant();
    rec_seq_facteur(LexC, Arbre1, table);
}



void rec_suite_seq_facteur(Lexeme LexC, Ast Arbre1, Ast *Arbre2, TAB_IDF *table){

    Ast Arbre3, Arbre4;
    TypeOperateur OP;
    LexC = lexeme_courant();
    switch (LexC.nature)
        {
            case MUL:
            case DIV:
                rec_op2(LexC, &OP);
                LexC = lexeme_courant();
                rec_facteur(LexC, &Arbre3, table);
                Arbre4 = creer_operation(OP, Arbre1, Arbre3);
                rec_suite_seq_facteur(LexC, Arbre4, Arbre2, table);
                break;
            
            default:
                *Arbre2 = Arbre1;
                break;
        }
}




void rec_op1(Lexeme LexC, TypeOperateur *OP){
    LexC = lexeme_courant();
    switch(LexC.nature) {
        case PLUS:
            *OP = N_PLUS;
            avancer(); 
            break;

        case MOINS:
            *OP = N_MOINS;
            avancer();
            break;

        default:
            printf("Lexeme faux\n");
            exit(0);
    }
}

void rec_op2(Lexeme LexC, TypeOperateur *OP){

    LexC = lexeme_courant();

    switch(LexC.nature){

        case MUL:
            *OP = N_MUL;
            avancer(); 
            break;

        case DIV:
            *OP = N_DIV;
            avancer();
            break;

        default:
            printf("Lexeme faux\n");
            exit(0);
    }
}

void rec_eag(Lexeme LexC, Ast *Arbre1, TAB_IDF *table){

    LexC = lexeme_courant();
    rec_seq_terme(LexC, Arbre1 , table);
}

void rec_inst(Lexeme LexC, Ast *Arbre1, TAB_IDF *table)
{
    char idft[100];
    Ast ArbG, ArbD, ArbSi, ArbAlors, ArbSinon, ArbBody = NULL;
    
    LexC = lexeme_courant();
    
    switch (LexC.nature) {
        
        
        // instruction de lecture
        case LIRE:
            // LIRE
            avancer();
            LexC = lexeme_courant();
            
            // PARO
            if (LexC.nature == PARO)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Il manque une parenthèse ouvrante pour lire.\nExemple Usage: lire(x);\n");
                exit(0);
            }
            
            // IDF
            if (LexC.nature == IDF)
            {
                strcpy(idft, LexC.chaine);
                ArbG = creer_identif(LexC.chaine);
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Il manque l'identificateur pour lire.\nExemple Usage: lire(x);\n");
                exit(0);
            }
            
            *Arbre1 = creer_lire(ArbG);
            
            // PARF
            if (LexC.nature == PARF)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Il manque une parenthèse fermante pour lire.\nExemple Usage: lire(x);\n");
                exit(0);
            }
            break;
        
        case IDF:
            strcpy(idft, LexC.chaine);
            ArbG = creer_identif(LexC.chaine);
            avancer();
            LexC = lexeme_courant();
            
            if (LexC.nature == AFF) {
                avancer();
            }
            else {
                printf("il manque un symbole d'Affectation \n");
                exit(0);
            }

            rec_eag(LexC, &ArbD, table);
            LexC = lexeme_courant();
            
            *Arbre1 = creer_aff(ArbG, ArbD);
            break;

        // instruction d'ecriture
        case ECRIRE:
            // ECRIRE
            avancer();
            LexC = lexeme_courant();
            
            // PARO
            if (LexC.nature == PARO)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Il manque une parenthèse ouvrante pour ecrire.\nExemple Usage: ecrire(x);\n");
                exit(0);
            }
            
            // EAG
            rec_eag(LexC, &ArbG, table);
            *Arbre1 = creer_ecrire(ArbG);
            
            LexC = lexeme_courant();
            
            // PARF
            if (LexC.nature == PARF)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Il manque une parenthèse fermante pour ecrire.\nExemple Usage: ecrire(x);\n");
                exit(0);
            }
            break;
            
        // instruction conditionnelle
        case SI:
            avancer();
            LexC = lexeme_courant();
            
            rec_condition(LexC, &ArbSi, table);
            LexC = lexeme_courant();
            
            // ALORS
            if (LexC.nature == ALORS)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Veuillez re-vérifier votre instruction\nIl manque le mot clé: alors \n");
                exit(0);
            }
            
            rec_seq_inst(LexC, &ArbAlors, table);
            LexC = lexeme_courant();
            
            // SINON
            if (LexC.nature == SINON)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Veuillez re-vérifier votre instruction\nIl manque le mot clé: sinon \n");
                exit(0);
            }
            rec_seq_inst(LexC, &ArbSinon, table);
            LexC = lexeme_courant();
            
            // FINSI
            if (LexC.nature == FINSINON)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Veuillez re-vérifier votre instruction\nIl manque le mot clé: finSi \n");
                exit(0);
            }
            
            *Arbre1 = creer_Si(ArbSi, ArbAlors, ArbSinon);
            break;
        
        // instruction iterative
        case TQ:
            avancer();
            LexC = lexeme_courant();
            
            rec_condition(LexC, &ArbSi, table);
            LexC = lexeme_courant();
            
            if (LexC.nature == FAIRE)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Veuillez re-vérifier votre instruction\nIl manque le mot clé: faire \n");
                exit(0);
            }
            
            rec_seq_inst(LexC, &ArbBody, table);
            LexC = lexeme_courant();
            
            // FINTQ
            if (LexC.nature == FINTANTQUE)
            {
                avancer();
                LexC = lexeme_courant();
            }
            else
            {
                printf("Veuillez re-vérifier votre instruction\nIl manque le mot clé: finTantque \n");
                exit(0);
            }
            
            *Arbre1 = creer_TQ(ArbSi, ArbBody);
            break;
            
        case SINON:
            rec_seq_inst(LexC, Arbre1, table);
            break;
        
        default:
            printf("Veuillez re-vérifier votre instruction\n");
            exit(0);
    }
}

//------------------------------------------------------------------------------

void rec_suite_seq_inst(Lexeme LexC, Ast Arbre1, Ast *Arbre2, TAB_IDF *table)
{
    Ast Arbre3;
    LexC = lexeme_courant();
    
    switch (LexC.nature)
    {
        case SEPINSTR:
            avancer();
            rec_seq_inst(LexC, &Arbre3, table);
            *Arbre2 = creer_sepinst(Arbre1, Arbre3);
            break;
        
        default:
            //epsilon
            break;
    }
}

//------------------------------------------------------------------------------

void rec_seq_inst(Lexeme LexC, Ast *Arbre1, TAB_IDF *table)
{
    Ast Arbre2;
    LexC = lexeme_courant();
    
    switch (LexC.nature)
    {
        case SI:
        case TQ:
        case IDF:
        case LIRE:
        case ECRIRE:
            rec_inst(LexC, &Arbre2, table);
            rec_suite_seq_inst(LexC, Arbre2, Arbre1, table);
            break;
        
        case SINON: // retour a la reconnaissance d'instruction conditionnelle
        case FINSINON:
        case FINTANTQUE:
            break;
            
        case FIN_SEQUENCE: // epsilon
            break;
        
        default:
            printf("ERREUR DANS VOTRE INSTRUCTION\n");
            exit(0);
    }
}

//------------------------------------------------------------------------------

void rec_condition(Lexeme LexC, Ast *Arbre1, TAB_IDF *table)
{
    Ast ArbG, ArbD;
    TypeCompar comp_operateur;
    
    LexC = lexeme_courant();
    rec_eag(LexC, &ArbG, table);
    
    LexC = lexeme_courant();
    switch (LexC.nature)
    {
        
            
        case NEQUAL:
            comp_operateur = N_NEQUAL;
            avancer();
            break;

        case EQUAL:
            comp_operateur = N_EQUAL;
            avancer();
            break;

        case GREATER:
            comp_operateur = N_GREATER;
            avancer();
            break;
            
        case LESS:
            comp_operateur = N_LESS;
            avancer();
            break;
            
        case SUPEQUAL:
            comp_operateur = N_SUPEQUAL;
            avancer();
            break;
            
        case INFEQUAL:
            comp_operateur = N_INFEQUAL;
            avancer();
            break;
        
        default:
            printf("Operateur de comparaison non reconnu\n");
            exit(0);
    }
    
    LexC = lexeme_courant();
    rec_eag(LexC, &ArbD, table);
    
    *Arbre1 = creer_comp(comp_operateur, ArbG, ArbD);
}

//------------------------------------------------------------------------------

void analyser(char *fichier, Ast *Arbre1, TAB_IDF *table){
    Lexeme LexC;
    demarrer(fichier);
    LexC = lexeme_courant();
    rec_seq_inst(LexC, Arbre1, table);
    printf("\nAnalyse terminé\n");
    printf("\nRésultat: ");
    if (fin_de_sequence())
    {
        printf(" LEXEME VALIDE\n\n");
    }
    else
    {
        printf(" LEXEME FAUX\n\n");
        exit(0);
    }
}


void rec_facteur(Lexeme LexC, Ast *Arbre1, TAB_IDF *table){

    LexC = lexeme_courant();

    switch(LexC.nature){
        case ENTIER:
            *Arbre1 = creer_valeur(LexC.valeur);
            avancer();
            break;
        
        case PARO:
            avancer();
            LexC = lexeme_courant();
            rec_eag(LexC, Arbre1, table);
            
            LexC = lexeme_courant();
            
            if (LexC.nature == PARF)
            {
                avancer();
            }
            else
            {
                printf("Parenthèse fermante manquante !\n");
                exit(0);
            }
            break;
        
        case IDF:
            *Arbre1 = creer_identif(LexC.chaine);
            avancer();
            break;
        
        default:
            printf("Erreur syntaxe !\n");
            exit(0);
    }
}