
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include <global.h>
#include <notify.h>
#include <lex.h>

enum {INIT, BLANC, VIRGULE, HEXA, DECIMAL, DECIMAL_ZERO, SYMBOLE, DEUX_POINTS, PARENTHESE_O,PARENTHESE_F, COMMENTAIRE, REGISTRE, DIRECTIVE}


int main(){
    int c;          /* caractere analyse*/
    int S = INIT /* etat de l'automate*/
    char
    FILE *pf;

    if(pf = fopen("nom_du_fichier","rt")==NULL){
        perror("erreur d'ouverture fichier"); return 1;
    }
    while(EOF!=(c=fgetc(pf))){
        switch(S){
        case INIT:
            i=0;
            if(isdigit(c)) S= (c=="0")? DECIMAL_ZERO : DECIMAL;
            else if(isspace(c)) S=BLANC;/*isspace regrade si c'est un blanc,trier apres*/
            else if(c=="#") S=COMMENTAIRE;
            else if(c==",") S=VIRGULE;
            else if(c=="$") S=REGISTRE;  /* faire un tableau pour la cerise sur le gateaux*/
            else if(c==".") S=DIRECTIVE;
            else if(c==":") S=DEUX_POINTS;
            else if(c=="(") S=PARENTHESE_O;
            else if(c==")") S=PARENTHESE_F;
            /*else if(c=="\n") S=NL;*/
            else if(isalpha(c)) S=SYMBOLE;
            else if(c=EOF) return 0 /*fin du fichier*/
            else perror("caractere invalide");
            break;
        case DECIMAL_ZERO:/*on verifie le prefixe de l'hexa*/
            if (c=="x") S=HEXA;
            else if(isspace(c)||c==EOF) S=BLANC;/* stocker sous forme de lexeme*/
            else if(c==",") S=VIRGULE;
            else perror("caractere invalide");
            break;
        case HEXA:
            if(isdigit(c)) S=HEXA;
            else if(c==EOF || isspace(c)) S=BLANC;
            else if(c==",") S=VIRGULE;
            else perror("caractere invalide");
            break;
        case DECIMAL:
            if(isdigit(c)) S=DECIMAL;
            else if(isspace(c)||c==EOF) S=BLANC;
            else if(c==",") S=VIRGULE;
            else perror("caractere invalide");
            break;
        case COMMENTAIRE:
            if(c=="\n" || c==EOF) S=BLANC;
            else S=COMMENTAIRE;
        case VIRGULE:
            S= INIT; /* stocker la virgule*/
        case SYMBOLE:
            if(isalpha(c)) S=SYMBOLE;
            else if(isspace(c)||c==EOF) S=BLANC;
            else if(c==",") S=VIRGULE;
            else perror("caractere invalide");
            break;
        case DEUX_POINTS:
            S=INIT;
        case PARENTHESE_F:
            S=INIT;
        case PARENTHESE_O:
            S=INIT;
        case REGISTRE:
            if(isspace(c)||c==EOF) S=BLANC;
            else if(isalnum(c)) S=REGISTRE;
            else if(c==",") S=VIRGULE;
            else perror("caractere invalide");
            break;
        case DIRECTIVE:
            if(isalpha(c)) S=DIRECTIVE;
            else if(isspace(c)||c==EOF) S=BLANC;
            else perror("caractere invalide");
            break;
        }
    }
        return 0
}
