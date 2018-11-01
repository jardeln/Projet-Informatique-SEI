#ifndef _GRAM_H_
#define _GRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <lex.h>



typedef union
{
	char val1;    
	int val2;
	char* val3;
	unsigned int val4;	
	unsigned char val5;
	
} OPERANDE;


/*--------------------structures section text------------------------------*/



struct text                                /* maillon de la collection text */ 
{             
	char* chaine;
	unsigned char NB_op;
	int ligne;
	int decalage;
/*	LISTE_OPERAN *operan_1;                     maximum de 3 operandes et chacun peut comporter plusieurs lexèmes -200($7) mis en liste 
	LISTE_OPERAN *operan_2;
	LISTE_OPERAN *operan_3;                      optionnel  pour l'instant */
};
typedef struct text TEXT;



struct collec_text
{                
	TEXT val;                             /* collection d'instruction */
	struct collec_text *suiv ;
};
typedef struct collec_text COLLEC_TEXT;



/*--------------------structures section data------------------------------*/

	

struct liste_operan
{                                                  /* structure de la liste des lexemes dans l'operande X */ 
	OPERANDE valeur_operande;                  /* UTILISER val2 DANS LES FONCTIONS   */
	struct liste_operan *suiv ;
};
typedef struct liste_operan LISTE_OPERAN;
	

struct data                              /* maillon de la collection de donnes */ 
{
	char* chaine;
	unsigned char NB_op;
	int ligne;
	int decalage;
	LISTE_OPERAN *operan;             /* convertir les hex en dec cf fonction Nans*/
};
typedef struct data DATA;




struct collec_data                         /* collection de donnes */
{                
	DATA val;
	struct collec_data *suiv;
};
typedef struct collec_data COLLEC_DATA;



/*--------------------structures section bss------------------------------*/



struct bss                              /* maillon de la collection de donnes bss */ 
{
	char* chaine;
	unsigned char NB_op;
	int ligne;
	int decalage;
	int valeur;             
};
typedef struct bss BSS;



struct collec_bss                         /* collection de donnes bss */
{                
	BSS val;
	struct collec_bss *suiv ;
};
typedef struct collec_bss COLLEC_BSS;



/*--------------------structures des symboles------------------------------*/



struct symboles                              /* maillon de la collection de donnes bss */ 
{
	char* chaine;
	int ligne;
	int section;              /* ou mettre juste un numero de section (1 2 et 3)  */
	int decalage;
};
typedef struct symboles SYMBOLES;



struct collec_symb                        /* collection des symboles */
{                
	SYMBOLES val;
	struct collec_symb *suiv ;
};
typedef struct collec_symb COLLEC_SYMB;



/*---------------------structures du dictionnaire---------------------------*/


struct dico
{
	char inst[5];
	int nb_ope;
	char type1[2];
	char type2[2];
	char type3[2];
};
typedef struct dico DICO;



/*------------------------entêtes de fonctions------------------------------*/



LISTE_LEXEME* Instruction(LISTE_LEXEME*, COLLEC_TEXT*, int , DICO*);
void EcritureOP(int , TEXT* , LISTE_LEXEME* );
int OpAttendue(DICO* , int , int , int , LISTE_LEXEME* );
int ChercInst(DICO* , char* , int );
DICO* Dico(FILE* , int* );
void Gram(LISTE_LEXEME*);

#endif

