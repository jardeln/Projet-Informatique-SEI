
/**
 * @file main.c
 * @author François Portet <francois.portet@imag.fr> from François Cayre
 * @brief Main entry point for MIPS assembler.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include <global.h>
#include <notify.h>
#include <lex.h>
#include <gram.h>

/**
 * @param exec Name of executable.
 * @return Nothing.
 * @brief Print usage.
 *
 */
void print_usage( char *exec ) {
    fprintf(stderr, "Usage: %s file.s\n",
            exec);
}



/**
 * @param argc Number of arguments on the command line.
 * @param argv Value of arguments on the command line.
 * @return Whether this was a success or not.
 * @brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {

    unsigned int 	nlines 	= 0;
    char         	 *file 	= NULL;
	LISTE_LEXEME* Liste = NULL;

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche */
    WARNING_MSG("Un message WARNING_MSG !");

    /* macro INFO_MSG : uniquement si compilé avec -DVERBOSE. Cf. Makefile*/
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);

    /* macro DEBUG_MSG : uniquement si compilé avec -DDEBUG (ie : compilation avec make debug). Cf. Makefile */
    DEBUG_MSG("Un message DEBUG_MSG !");

    /* La macro suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */


    if ( argc <2 ) {
        print_usage(argv[0]);
        exit( EXIT_FAILURE );
    }


    file  	= argv[argc-1];


    if ( NULL == file ) {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        exit( EXIT_FAILURE );
    }



    /* ---------------- do the lexical analysis -------------------*/
    Liste = lex_load_file( file, &nlines, Liste );
    DEBUG_MSG("source code got %d lines",nlines);
    afficher_liste(Liste);
    Gram(Liste);

    /* ---------------- Free memory and terminate -------------------*/

    /* TODO free everything properly*/

    exit( EXIT_SUCCESS );
}

