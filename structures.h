                                         //ajouter les ifndef et define dans les dossiers .h pour éviter les boucles infinies

                                         // chaque lexeme est une chaine de caractère avec un numéro d indentifiant, numéro de ligne

struct LEXEME{
  int identifiant;
  int ligne;
  char* chaine;
  int valeur;                            // si c'est un nombre
};


struct LISTE_LEXEME{
  LEXEME val;
  LEXEME* suiv ;
};













