
/**
 * @file lex.c
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexical analysis routines for MIPS assembly syntax.
 *
 * These routines perform the analysis of the lexeme of an assembly source code file.
 */

#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <lex.h>
#include <gram.h>


/* ATTENTION: getNextToken est juste un exemple -> il est à recoder completement !!! */
/**
 * @param token The pointeur to the lexeme to be extracted.
 * @param current_line The address from which the analysis must be performed.
 * @return the address at which the analysis stopped or NULL if the analysis is terminated.  
 * @brief This function get an input line, extracts a token from it and return the pointeur to the next place where the analysis should continue. 
 */

/* note that MIPS assembly supports distinctions between lower and upper case*/
char* getNextToken(char** token, char* current_line) {
    char* start = current_line; 
    char* end=NULL; 
    int token_size=0;
    /*printf("current_line = %p\n", current_line);*/

    /* check input parameter*/ 
    if (current_line ==NULL) return NULL;

    /* eats any blanks before the token*/
    while (*start!='\0' && isblank(*start)){
	start++;
    }	
    /* go till next blank or end */ 
    end=start; 
    if (*end == ',' || *end == '(' || *end == ')' || *end== ':' || *end == '#'){ 				/* permet de séparer les séparateurs du reste de la chaine*/
    	end++;
	token_size=end-start;
	/*printf("token size = %d\n", token_size);*/
		*token 	= calloc(token_size+1,sizeof(*start));
		strncpy(*token,start,token_size);
		(*token)[token_size]='\0';       
		return end;
	}
   if (*end == 34){ 												/* permet de prendre la chaine de caractère entière avec les espaces et les chaines de*/ 
	end++;													/*caractères imbriquées, on utilise ici le code ASCII*/
	while(*end!='\0'){
		printf("*end = %c\n",*end);
		if (*end == 92 && *(end+1) == 34){
			end = end + 2;
		}
		else if(*end == 34){
			end++;
			goto saut; 										/* pour sortir de la boucle et éviter l'autre boucle*/
		}
		else {end++;}
	}
    }
    while (*end!='\0' && !isblank(*end) && *end!=',' && *end!='(' && *end!=')' && *end!=':' && *end != '#' ) { 	/* permet de séparer les séparateurs du reste de la chaine*/
	end++;
    }
saut:
    /*compute size : if zero there is no more token to extract*/ 	
    token_size=end-start;
    /*printf("token size = %d\n", token_size);*/
    if (token_size>0){
	*token 	= calloc(token_size+1,sizeof(*start));
	strncpy(*token,start,token_size);
	(*token)[token_size]='\0';       
	return end;
    } 
    return NULL;
}

LISTE_LEXEME* ajout(char* string, int id, LISTE_LEXEME *L, int nline){ 						/*foction qui ajoute en queue de liste*/
	LEXEME lex ;                                  
	lex.identifiant= id;
	lex.ligne = nline;
	lex.chaine= string;                                  
	LISTE_LEXEME *pm = calloc(1, sizeof(  *pm )) ;
	LISTE_LEXEME* pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
		
	else if(L == NULL){	
		pm->val = lex;	
		pm->suiv = NULL;
		L = pm;
		/*printf("ajouttete \n");*/
	}	
	else{	
		/*printf("1\n");*/
		pm->val = lex ;
		pm->suiv = NULL;
		pm2 = L;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
			/*printf("while\n");*/
		}	
		pm2->suiv = pm;	
	}
	return L;
}

void afficher_liste(LISTE_LEXEME *L){
	LISTE_LEXEME* pm=L;	
	while(pm !=NULL){
		if (pm->val.chaine[0] == '\n') printf("[NL] ");		
		else printf("%s ",pm->val.chaine);			
		printf("%d ",pm->val.identifiant);  
		printf("ligne=%d  \n",pm->val.ligne);
		printf("\n");
			
		/*printf("%d\n",pm->val.valeur);*/								/*sera utilisé plus tard dans la fonction*/
		pm = pm->suiv;
	}
}

/**
 * @param line String of the line of source code to be analysed.
 * @param nline the line number in the source code.
 * @return should return the collection of lexemes that represent the input line of source code.
 * @brief This function performs lexical analysis of one standardized line.
 *
 */

enum {INIT, VIRGULE, HEXA, DECIMAL, DECIMAL_ZERO, SYMBOLE, DEUX_POINTS, PARENTHESE_O,PARENTHESE_F, COMMENTAIRE, REGISTRE, DIRECTIVE, CHAINE}; /* liste de tous les lexemes*/

LISTE_LEXEME* lex_read_line( char *line, int nline, LISTE_LEXEME* Liste) {
    char* token = NULL;
    char* current_address=line;
    while( (current_address= getNextToken(&token, current_address)) != NULL)
    {  	
	int i = -1;         
        int S = INIT;
	while(i!=strlen(token)){										/*la boucle s'arrete quand on est arrivé à la fin de la chaine*/
        	switch(S)
			{
        		case INIT:
				i++;
				printf("token[i]=%c INIT\n",token[i]);
            			if(isdigit(token[i])) S = (token[i]=='0')? DECIMAL_ZERO : DECIMAL;
	    			else if(token[i] == '(') { S= INIT; Liste = ajout(token, 10,Liste,nline);}
				else if(token[i] == ')') { S= INIT; Liste = ajout(token, 11,Liste,nline);}
				else if(token[i] == ',') { S= INIT; Liste = ajout(token, 7,Liste,nline);}
				else if(token[i] == ':') { S= INIT; Liste = ajout(token, 4,Liste,nline);}
				else if(token[i] == '#'){S=COMMENTAIRE;}
				else if(token[i] == 34){S = CHAINE; printf(" c'est une chaine de caractère\n");}
            			else if(token[i]=='$') S=REGISTRE;
            			else if(token[i]=='.') S=DIRECTIVE;
            			else if(isalpha(token[i])) {S=SYMBOLE; printf("alpha\n");}
            			else if(token[i] == '\0') break;
            			else WARNING_MSG("Un Caractère invalide Init !");
            			break;
        		case DECIMAL_ZERO:									/*on verifie le prefixe de l'hexa*/
            			i++;
            			if (token[i]=='x') S=HEXA;
            			else if (isdigit(token[i])) S = DECIMAL;
            			else WARNING_MSG("Un Caractère  invalide ligne %d : \"%s\" pour un type DECIMAL_ZERO !", nline,token); /* on utilise des warnig pour tester un grand nombre d'erreurs à la fois*/
            			break;
        		case HEXA:
            			i++;
			    	if(isdigit(token[i])) S=HEXA;
			    	else if(token[i]=='\0') Liste = ajout(token, 9, Liste,nline);
			    	else WARNING_MSG("Un Caractère invalide ligne %d : \"%s\" pour un type HEXA !", nline,token);
			    	break;
			case DECIMAL:
			    	i++;
				printf("token[i]=%c decimal\n",token[i]);
			    	if(isdigit(token[i])) S=DECIMAL;
			    	else if(i==strlen(token)) Liste = ajout(token, 6, Liste,nline);
			    	else WARNING_MSG("Un Caractère invalide ligne %d : \"%s\" pour un type DECIMAL !", nline,token);
			    	break;
			case COMMENTAIRE: 
				printf("Symbole\n");
			    	Liste = ajout(token,1,Liste,nline);
				return Liste;
			case SYMBOLE:
			    	i++;
				printf("token[i]=%c longueur = %u i = %d\n",token[i],(unsigned)strlen(token),i);
			    	if(isalpha(token[i])) S=SYMBOLE;
			    	else if(i==strlen(token))
					{
					printf("ecriture\n");
					Liste = ajout(token, 3, Liste,nline);
					}
			    else WARNING_MSG("Un Caractère invalide ligne %d : \"%s\" pour un type SYMBOLE !", nline,token);
			    break;
			case REGISTRE:
			    	i++;
			    	if(i==strlen(token)) Liste = ajout(token, 8, Liste,nline);
			    	else if(isalnum(token[i])) S=REGISTRE;
			    	else WARNING_MSG("Un Caractère invalide ligne %d : \"%s\" pour un type REGISTRE !", nline,token);
			    	break;
			case DIRECTIVE:
			    	i++;
			    	if(isalpha(token[i])) S=DIRECTIVE;
			    	else if(i==strlen(token)) Liste = ajout(token, 5, Liste,nline);
			   	else WARNING_MSG("Un Caractère invalide ligne %d : \"%s\" pour un type DERICTIVE !", nline,token);
			    	break;
			case CHAINE:
				i++;
				if(token[i] == '\0'){	
					printf("CHAINE\n");
					Liste = ajout(token,12,Liste,nline);
				}
				else S = CHAINE;
				break;
				}
			
		    	}	
				printf("%d ",Liste->val.identifiant);  
				printf("%d ",Liste->val.ligne);
				printf("%s \n",Liste->val.chaine);		
				/*printf("%d\n",Liste->val.valeur);*/
				/*puts(token);*/
				/*Liste = ajout("\n",2,Liste);  à la fin de la ligne il y a un retour à la ligne*/
		}
	/*Liste = ajout("\n",2,Liste);  à la fin de la ligne il y a un retour à la ligne*/
    	return Liste;
}

/**
 * @param file Assembly source code file name.
 * @param nlines Pointer to the number of lines in the file.
 * @return should return the collection of lexemes
 * @brief This function loads an assembly code from a file into memory.
 *
 */
LISTE_LEXEME* lex_load_file( char *file, unsigned int *nlines,LISTE_LEXEME* Liste ) 
{

    FILE        *fp   = NULL;
    char         line[STRLEN]; /* original source line */



    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) ) {
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;

            if ( 0 != strlen(line)) {
                Liste = lex_read_line(line,*nlines, Liste);
            }
	Liste = ajout("\n",2,Liste,*nlines);  										/*à la fin de la ligne il y a un retour à la ligne, mais la fonction met un retour*/ 																/*à la ligne pour la dernière ligne même si il n'y en a pas*/
        }
    }

    fclose(fp);
    return Liste;
}



