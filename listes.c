#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "listes.h"


liste_t creer_liste(void) {
    return NULL;
}


LISTE_LEXEME ajout(char* string, int id, LISTE_LEXEME L){    //prend la chaine de caractère et sa catégorie et crée lexeme 
    struct LEXEME lexeme;                                    // puis l'empile à la liste de lexeme
    lexeme.identifiant= id;
    lexeme.chaine= string;                                   // ATTRIBUTION D' UN POINTEUR ?????
    struct LEXEME * pm = calloc(1, sizeof( * pm )) ;
    if(pm == NULL) {
        printf("erreur allocation ajout tete\n");
        return NULL;
    }
    pm->val = lexeme;
    pm->suiv = L;
    return pm;
}





int compter(LISTE_LEXEME L) {
    LISTE_LEXEME p = NULL; // toujours bien d'initialiser les pointeurs des leur declaration...
    int nb = 0;
    p = L;
    while( p != NULL) {
        nb ++ ;
        p = p->suiv;
    }
    return nb;
}



// PRECONDITION : liste non vide
LEXEME  obtenir_tete(LISTE_LEXEME L) {
    // Principe de la prÃ©condition : comme la fonction
    // n'a pas de mecanisme de retour pour les erreurs,
    // on decide qu'elle "fait np quoi" (=>sort brutalement du programme)
    // si la precondition n'est pas respectee
    if(est_vide(L)) {
        printf("obtenir_tete() -> LISTE VIDE : arret brutal programme\n");
        exit(1);
    }
    return L->val;
}


