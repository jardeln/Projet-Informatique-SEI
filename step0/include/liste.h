#ifndef _LISTE_H_
#define _LISTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <lex.h>
#include <gram.h>


COLLEC_TEXT* ajout_text(char* , unsigned char , int , int , COLLEC_TEXT *);
void afficher_collec_text(COLLEC_TEXT* );
LISTE_OPERAN* ajout_operande( char* , LISTE_OPERAN * );
COLLEC_DATA* ajout_data(char* , unsigned char ,  int , int , LISTE_OPERAN *, COLLEC_DATA *);
void afficher_collec_data(COLLEC_DATA* );
COLLEC_SYMB* ajout_symbole(char* , int , int , int , COLLEC_SYMB* );
void afficher_collec_symb(COLLEC_SYMB* );
COLLEC_BSS* ajout_bss(char* , unsigned char , int , int , int , COLLEC_BSS* );
void afficher_collec_bss(COLLEC_BSS* );

#endif
