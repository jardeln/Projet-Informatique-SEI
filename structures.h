#ifndef _STRUCTURE_H
#define _STRUCTURE_H


struct LEXEME{                           // chaque lexeme est une chaine de caractère avec un numéro d indentifiant, numéro de ligne
  int identifiant;
  int ligne;
  char* chaine;
  int valeur;                            // si c'est un nombre
};


struct LISTE_LEXEME{                 // chaine de lexeme 
  LEXEME val;
  LEXEME* suiv ;
};





#endif


