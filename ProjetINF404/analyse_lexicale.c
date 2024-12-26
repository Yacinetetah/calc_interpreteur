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
#include <math.h>
#include <string.h>
#include <stdlib.h>  
#include <stdio.h> 

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

#define NB_MOTCLE 9
char motCle[NB_MOTCLE][20] = {"lire", "ecrire", "si", "alors", "sinon", "finsi", "tantque", "faire", "fintantque"};

   /* --------------------------------------------------------------------- */

 
/* Les variables et fonctions locales au module */

   Lexeme lexeme_en_cours ;	/* le lexeme courant */

   void ajouter_caractere (char *s, char c);
   Nature_Caractere nature_caractere (char c);
   int est_separateur(char c ) ;
   int est_chiffre(char c ) ;
   int est_symbole(char c ) ;
   int est_sepinst(char c);
   int est_aff(char c);
   int est_alpha(char c);
   int est_bool(char c);
   int est_comp(char c);
   void reconnaitre_lexeme();

   /* --------------------------------------------------------------------- */

   void demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      avancer();
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   } 

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() {
      return lexeme_en_cours;
   } 

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   }

   /* --------------------------------------------------------------------- */


   // reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme

   void reconnaitre_lexeme() {
      typedef enum {E_INIT, E_ENTIER, E_FIN, E_IDF, E_EQUAL, E_NEQUAL, E_COMP, E_ET, E_OU} Etat_Automate ;
      Etat_Automate etat=E_INIT;

     // on commence par lire et ignorer les separateurs
     while (est_separateur(caractere_courant())) {
        	avancer_car() ;
     } ;
     lexeme_en_cours.chaine[0] = '\0' ;

     // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme
     

     /*Le code représente un analyseur lexical pour un langage de programmation. 
     Il utilise une boucle principale pour parcourir les caractères d'une séquence jusqu'à ce que l'état soit E_FIN.
      À chaque itération, il évalue la nature du caractère courant et effectue différentes actions en fonction de cette nature :
       identification de la fin de la séquence, 
      traitement des chiffres pour les entiers, des lettres pour les identificateurs, des symboles, des séparateurs,
       des opérateurs d'affectation,
       des booléens et des opérateurs de comparaison. Si la nature du caractère n'est pas reconnue, 
       une erreur est signalée et le programme se termine. 
     Chaque cas déclenche une transition d'état vers E_FIN lorsque le traitement du lexème en cours est terminé.*/
    while (etat != E_FIN) {

	    switch (etat) {

		  case E_INIT: // etat initial

			switch(nature_caractere(caractere_courant())) {

				case C_FIN_SEQUENCE: 
             		lexeme_en_cours.nature = FIN_SEQUENCE;
              	etat = E_FIN;
					break ;


				case CHIFFRE: 
		     		      lexeme_en_cours.nature = ENTIER;
                	lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
     		          ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		lexeme_en_cours.valeur = caractere_courant() - '0';
               		etat = E_ENTIER;
		   			      avancer_car() ;
					break ;

        case LETTRE:
              lexeme_en_cours.nature = IDF;
              lexeme_en_cours.ligne = numero_ligne();
              lexeme_en_cours.colonne = numero_colonne();
          		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
              etat = E_IDF;
              avancer_car();
          break;

				case SYMBOLE: 
		       		lexeme_en_cours.ligne = numero_ligne();
         		  lexeme_en_cours.colonne = numero_colonne();
		       		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		switch (caractere_courant()) {
               		  case '+':
               			  lexeme_en_cours.nature = PLUS;
               			  etat = E_FIN;
			   			        break;
               		  case '-':
               			  lexeme_en_cours.nature = MOINS;
               			  etat = E_FIN;
	   			            break;
               		  case '*':
            			    lexeme_en_cours.nature = MUL;
               			  etat = E_FIN;
   			              break;
                    case '/':
                       lexeme_en_cours.nature = DIV;
                       etat = E_FIN;
                       break;
                    case '(':
                        lexeme_en_cours.nature = PARO;
                        etat = E_FIN;
                        break;
                    case ')':
                        lexeme_en_cours.nature = PARF;
                        etat = E_FIN;
                        break;                     
		       		  default:
						            printf("Erreur_Lexicale\n") ;
				 		            exit(0) ;
				 	      } ;
		   		      avancer_car() ;
					      break ;
        case SEPARATEUR:
                        lexeme_en_cours.nature = SEPINSTR;
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN;
          		   		    avancer_car() ;
                        break;
                   
        case AFFEC:
                        lexeme_en_cours.nature = AFF;
                        lexeme_en_cours.ligne = numero_ligne();
                        lexeme_en_cours.colonne = numero_colonne();
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_EQUAL;
          		   		    avancer_car();
                        break;
     
        case BOOL:
                        lexeme_en_cours.ligne = numero_ligne();
                     	  lexeme_en_cours.colonne = numero_colonne();
            		        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                 	      switch (caractere_courant())
                        {
                            case '&':
                                etat = E_ET;
                                break;
                            
                            case '|':
                                etat = E_OU;
                                break;
                            
                            case '!':
                                etat = E_NEQUAL;
                                lexeme_en_cours.nature = NON;
                                break;
                            
                            default:
       		                      printf("Erreur: lexique faux\n");
           		                  exit(0);
                        }
                        avancer_car();
                        break;
                    
        case COMP:
                        switch (caractere_courant())
                        {
                            case '<':
                                lexeme_en_cours.nature = LESS;
                                break;
                            
                            case '>':
                                lexeme_en_cours.nature = GREATER;
                                break;
                            
                            default:
       		                      printf("Erreur: lexique faux\n");
           		                  exit(0);
                        }
                        lexeme_en_cours.ligne = numero_ligne();
                     	  lexeme_en_cours.colonne = numero_colonne();
            		        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_COMP;
                        avancer_car();
                        break;

				default:
		           printf("Erreur: lexique faux\n") ;
		           exit(0) ;
		} ;
		break ;

		case E_ENTIER:  // reconnaissance d'un entier
			switch(nature_caractere(caractere_courant())) {
			   case CHIFFRE:
		  			         ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  	   lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 + caractere_courant() - '0';
                  	   etat = E_ENTIER;
                  	   avancer_car ();
					         break ;
          default:
                  	    etat = E_FIN;
                        break;
      };
      case E_IDF:
        switch(nature_caractere(caractere_courant()))
                {
                    case LETTRE:
                    case CHIFFRE:
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_IDF;
                        avancer_car();
                        break;
                    
                    default:
                        for (int i=0 ; i<NB_MOTCLE ; i++)
                        {
                            if (strcmp(lexeme_en_cours.chaine, motCle[i]) == 0)
                            {
                                switch(i)
                                {
                                    case 0:
                                        lexeme_en_cours.nature = LIRE;
                                        break ;
                                    case 1:
                                        lexeme_en_cours.nature = ECRIRE;
                                        break ;
                                    case 2:
                                        lexeme_en_cours.nature = SI;
                                        break;
                                    case 3:
                                        lexeme_en_cours.nature = ALORS;
                                        break;
                                    case 4:
                                        lexeme_en_cours.nature = SINON;
                                        break;
                                    case 5:
                                        lexeme_en_cours.nature = FINSINON;
                                        break;
                                    case 6:
                                        lexeme_en_cours.nature = TQ;
                                        break;
                                    case 7:
                                        lexeme_en_cours.nature = FAIRE;
                                        break;
                                    case 8:
                                        lexeme_en_cours.nature = FINTANTQUE;
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    
                        etat = E_FIN;
                        break;
                }
                break;
                
      case E_EQUAL:
              switch(nature_caractere(caractere_courant()))
              {
                    case AFFEC:
                        lexeme_en_cours.nature = EQUAL;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN;
                        avancer_car();
                        break;
                    
                    default:
                        etat = E_FIN;
                        break;
              }
              break;
          
      case E_NEQUAL:
              switch(nature_caractere(caractere_courant()))
              {
                    case AFFEC:
                        lexeme_en_cours.nature = NEQUAL;
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN;
                        avancer_car();
                        break;
                    
                    default:
                        etat = E_FIN;
                        break;
              }
              break;
          
      case E_COMP:
              switch(nature_caractere(caractere_courant()))
              {
                    case AFFEC:
                        switch(lexeme_en_cours.nature)
                        {
                            case GREATER:
                                lexeme_en_cours.nature = SUPEQUAL;
                                break;
                            
                            case LESS:
                                lexeme_en_cours.nature = INFEQUAL;
                                break;
                            
                            default:
       		                      printf("Erreur: Lexique faux\n");
           		                  exit(0);
                        }
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        etat = E_FIN;
                        avancer_car();
                        break;
                    
                    default:
                        etat = E_FIN;
                        break;
              }
              break;
          
          case E_ET:
              if (caractere_courant() == '&')
              {
                  lexeme_en_cours.nature = ET;
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FIN;
                  avancer_car();
                  break;
              }
              else
              {
                  printf("Erreur: Lexique faux\n");
                  exit(0);
              }
              break;
          
          case E_OU:
              if (caractere_courant() == '|')
              {
                  lexeme_en_cours.nature = OU;
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FIN;
                  avancer_car();
                  break;
              }
              else
              {
                  printf("Erreur: Lexique faux\n");
                  exit(0);
              }
              break;
	    case E_FIN:  // etat final
		break ;
	    
	   } ; // fin du switch(etat)
	} ; // fin du while (etat != fin)
};

   /* --------------------------------------------------------------------- */

   // cette fonction ajoute le caractere c a la fin de la chaine s 
   // (la chaine s est donc modifiee)
 
   void ajouter_caractere (char *s, char c) {
	int l ;
	
	l = strlen(s) ;
	s[l] = c ;
	s[l+1] = '\0' ;
   } ;

   /* --------------------------------------------------------------------- */

   Nature_Caractere nature_caractere (char c) {
	   if (fin_de_sequence_car(c)) return C_FIN_SEQUENCE;
	   if (est_chiffre(c)) return CHIFFRE;
	   if (est_symbole(c)) return SYMBOLE;
     if (est_alpha(c)) return LETTRE;
     if (est_aff(c)) return AFFEC;
     if (est_sepinst(c)) return SEPARATEUR;
     if (est_bool(c)) return BOOL;
     if (est_comp(c)) return COMP;
	   return ERREUR_CAR ;
   }
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere separateur
   int est_separateur(char c) { 
      return c == ' ' || c == '\t' || c == '\n' ;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere chiffre
   int est_chiffre(char c) {
      return c >= '0' && c <= '9' ;
   }
   /* --------------------------------------------------------------------- */
   
   int est_sepinst(char c) { 
      return c == ';';
   }
   
   /* --------------------------------------------------------------------- */
   
   int est_aff(char c) { 
      return c == '=';
   }
   
   /* --------------------------------------------------------------------- */
   
   int est_alpha(char c)
   {
       return ( ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') );
   }
   
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
   int est_symbole(char c)  {
      switch (c) {
         case '+':  
	 	     case '-':  
	 	     case '*':
	 	     case '/':
         case '(':
         case ')':
            return 1;
        default:
            return 0;
      } 
   }
   
   /* --------------------------------------------------------------------- */
   
   int est_bool(char c) {
       return c == '|' || c == '&' || c == '!';
   }
   
   /* --------------------------------------------------------------------- */
   
   int est_comp(char c) { 
      return c == '>' || c == '<' ;
   }
   
   

   /* --------------------------------------------------------------------- */

   // renvoie la chaine de caracteres correspondant a la nature du lexeme
   char *Nature_vers_Chaine (Nature_Lexeme nature) {
	    switch (nature) {
		      case ENTIER: return "ENTIER" ;
		      case PLUS: return "PLUS" ;
		      case MOINS: return "MOINS" ;            
		      case MUL: return "MUL" ;
          case DIV: return "DIV" ;  
          case PARO: return "PARO" ;
          case PARF: return "PARF" ;   
          case AFF: return "AFF";
          case SEPAFF: return "SEPAFF";
          case SEPINSTR: return "SEPINSTR";
          case IDF: return "IDF";
          case LIRE: return "LIRE";
          case ECRIRE: return "ECRIRE";
          case SI: return "SI";
          case ALORS: return "ALORS";
          case SINON: return "SINON";
          case FINSINON: return "FINSINON";
          case TQ: return "TANTQUE";
          case FAIRE: return "FAIRE";
          case FINTANTQUE: return "FINTANTQUE";
          case EQUAL: return "EQUAL";
          case NEQUAL: return "NEQUAL";
          case GREATER: return "GREATER";
          case LESS: return "LESS";
          case SUPEQUAL: return "SUP OU EGALE";
          case INFEQUAL: return "INF OU EGALE";
          case ET: return "ET";
          case OU: return "OU";
          case NON: return "NON";     
    		  case FIN_SEQUENCE: return "FIN_SEQUENCE" ;     
      		default: return "ERREUR" ;            
	    } ;
   } 

   /* --------------------------------------------------------------------- */

   // affiche a l'ecran le lexeme l
   void afficher(Lexeme l) {

      switch (l.nature) {
         case FIN_SEQUENCE: 
            break;
         default: 
            printf("(ligne %d, ", l.ligne);
            printf("colonne %d) : ",l.colonne);
	    printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            switch(l.nature) {
                 case ENTIER:
                      printf("valeur = %d", l.valeur);
                 default:
                      break;
            } ;
	    printf("]") ;
	  } ;
 }

   /* --------------------------------------------------------------------- */

