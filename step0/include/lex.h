/**
 * @file lex.h
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexem-related stuff.
 *
 * Contains lexem types definitions, some low-level syntax error codes,
 * the lexem structure definition and the associated prototypes.
 */

#ifndef _LEX_H_
#define _LEX_H_


#include <stdio.h>

struct lexeme
{                          
	int identifiant;
	int ligne;
	char* chaine;
	int valeur;                            
};
typedef struct lexeme LEXEME;

struct liste_lexeme
{                
	LEXEME val;
	struct liste_lexeme *suiv ;
};
typedef struct liste_lexeme LISTE_LEXEME;

LISTE_LEXEME* lex_read_line( char*, int , LISTE_LEXEME*);
LISTE_LEXEME*	lex_load_file( char *, unsigned int * , LISTE_LEXEME*);
char* 	getNextToken( char** , char* );
LISTE_LEXEME* ajout(char*, int , LISTE_LEXEME*, int);
void afficher_liste(LISTE_LEXEME*);

#endif /* _LEX_H_ */

