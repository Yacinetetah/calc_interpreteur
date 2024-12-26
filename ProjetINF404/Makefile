
all: test_lexeme calculette

ast_construction.o: ast_construction.c type_ast.h
	gcc -g -Wall -c ast_construction.c
 
test_tabsym:  test_tabsym.o table_symbole.o
	gcc -g -Wall -o test_tabsym test_tabsym.o table_symbole.o

ast_parcours.o: ast_parcours.c type_ast.h
	gcc -g -Wall -c ast_parcours.c

analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h
	gcc -g -Wall -c analyse_lexicale.c 
 
analyse_syntaxique.o : analyse_syntaxique.c  analyse_syntaxique.h  analyse_lexicale.h lecture_caracteres.h ast_construction.h type_ast.h ast_parcours.h table_symbole.h
	 gcc -g -Wall -c analyse_syntaxique.c

lecture_caracteres.o: lecture_caracteres.h lecture_caracteres.c
	gcc -g -Wall -c lecture_caracteres.c

test_lexeme.o: analyse_lexicale.h
	gcc -g -Wall -c test_lexeme.c
 
calculette.o:  analyse_lexicale.h analyse_syntaxique.h table_symbole.h
	gcc -g -Wall -c calculette.c
 
table_symbole.o: table_symbole.c table_symbole.h
	gcc -g -Wall -c table_symbole.c
 
test_tabsym.o: table_symbole.c table_symbole.h
	gcc -g -Wall -c test_tabsym.c
 
calculette: analyse_lexicale.o analyse_syntaxique.o calculette.o lecture_caracteres.o ast_construction.o ast_parcours.o table_symbole.o
	gcc -g -Wall -o calculette analyse_lexicale.o analyse_syntaxique.o calculette.o lecture_caracteres.o ast_construction.o ast_parcours.o table_symbole.o
 
test_lexeme: analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o
	gcc -g -Wall -o test_lexeme analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o

clean:
	rm -f *.o calculette test_lexeme


 