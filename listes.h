#ifndef _ANALYSE_LEXICAL_H
#define _ANALYSE_LEXICAL_H  
                                         




// fonctions basiques pour les listes

LISTE_LEXEME creer_liste( void );
int est_vide( LISTE_LEXEME );
LISTE_LEXEME ajout_tete( LEXEME lexeme, LISTE_LEXEME L);

// les listes doivent etres non vides
LEXEME obtenir_tete(LISTE_LEXEME L);
LISTE_LEXEME supp_tete( LISTE_LEXEME L);
void visualiser( LISTE_LEXEME L);
int compter( LISTE_LEXEME L);




#endif


