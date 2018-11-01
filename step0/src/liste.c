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




COLLEC_TEXT* ajout_text(char* strg, unsigned char nbr_operande,  int nline, int decal, COLLEC_TEXT *C){
	TEXT txt ;                                  
	txt.chaine = strg;
	txt.NB_op = nbr_operande;
	txt.ligne = nline; 
	txt.decalage = decal;                                  
	COLLEC_TEXT *pm = calloc(1, sizeof(  *pm )) ;
	COLLEC_TEXT* pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
	else if(C == NULL){	
		pm->val = txt;	
		pm->suiv = NULL;
		C = pm;
	}	
	else{
		pm->val = txt ;
		pm->suiv = NULL;
		pm2 = C;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
		}	
		pm2->suiv = pm;	
	}
	return C;
}





void afficher_collec_text(COLLEC_TEXT* C){
	COLLEC_TEXT* pm=C;	
	while(pm !=NULL){		
		printf("%s \n",pm->val.chaine);			
		printf("nbr_operandes=%hhu \n",pm->val.NB_op);  
		printf("ligne=%d  \n",pm->val.ligne);
		printf("decalage=%d \n", pm->val.decalage);
		printf("\n");
		pm = pm->suiv;
	}
}

LISTE_OPERAN* ajout_operande( char* valorem, LISTE_OPERAN * L){
	OPERANDE ope ;                                  
	ope.val3 = valorem;                                                      /* à modifier en fonction de UNION */       
	LISTE_OPERAN *pm = calloc(1, sizeof(  *pm )) ;
	LISTE_OPERAN *pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
	else if(L == NULL){	
		pm->valeur_operande = ope;	
		pm->suiv = NULL;
		L = pm;
	}	
	else{
		pm->valeur_operande = ope ;
		pm->suiv = NULL;
		pm2 = L;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
		}	
		pm2->suiv = pm;	
	}
	return L;
}


COLLEC_DATA* ajout_data(char* strg, unsigned char nbr_operande,  int nline, int decal, LISTE_OPERAN *liste, COLLEC_DATA *C){
	DATA dt ;                                  
	dt.chaine = strg;
	dt.NB_op = nbr_operande;
	dt.ligne = nline; 
	dt.decalage = decal;
	dt.operan= liste;                               
	COLLEC_DATA *pm = calloc(1, sizeof(  *pm )) ;
	COLLEC_DATA* pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
	else if(C == NULL){	
		pm->val = dt;	
		pm->suiv = NULL;
		C = pm;
	}	
	else{
		pm->val = dt ;
		pm->suiv = NULL;
		pm2 = C;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
		}	
		pm2->suiv = pm;	
	}
	return C;
}

void afficher_collec_data(COLLEC_DATA* C){
	COLLEC_DATA* pm=C;	
	while(pm !=NULL){		
		printf("%s \n",pm->val.chaine);			
		printf("nbr_operandes=%hhu \n",pm->val.NB_op);  
		printf("ligne=%d  \n",pm->val.ligne);
		printf("decalage=%d \n", pm->val.decalage);
		LISTE_OPERAN* pl=pm->val.operan;                             /* pas sûr, pointeur dans la structure*/		
		printf("operandes= ");
		while(pl != NULL){
			printf("%s ", pl->valeur_operande.val3);          /*à modifier */
			pl = pl->suiv;
		}
		pm = pm->suiv;
		printf("\n");
		printf("\n");
	}
}


COLLEC_SYMB* ajout_symbole(char* strg, int n_l, int sect, int dec, COLLEC_SYMB* C){
	SYMBOLES symb ;                                  
	symb.chaine= strg;
	symb.ligne = n_l;
	symb.section= sect;
	symb.decalage=dec;                                  
	COLLEC_SYMB *pm = calloc(1, sizeof(  *pm )) ;
	COLLEC_SYMB* pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
		
	else if(C == NULL){	
		pm->val = symb;	
		pm->suiv = NULL;
		C = pm;
	}	
	else{
		pm->val = symb ;
		pm->suiv = NULL;
		pm2 = C;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
		}	
		pm2->suiv = pm;	
	}
	return C;
}

void afficher_collec_symb(COLLEC_SYMB* C){
	COLLEC_SYMB* pm=C;	
	while(pm !=NULL){	
		printf("%s \n",pm->val.chaine);			
		printf("ligne=%d \n",pm->val.ligne);
		if (pm->val.section == 1){
			printf("section TEXT\n"); 
		}
		if (pm->val.section == 2){
			printf("section DATA\n"); 
		}
		if (pm->val.section == 3){
			printf("section BSS\n"); 
		}
		else{
			printf("*****************probleme attribution section*****************\n");
		}
		printf("decalage=%d  \n",pm->val.decalage);
		printf("\n");
		pm = pm->suiv;
	}
}

COLLEC_BSS* ajout_bss(char* strg, unsigned char n_op, int n_l, int dec, int valorem, COLLEC_BSS* C){
	BSS bss ;                                  
	bss.chaine = strg;
	bss.NB_op = n_op;
	bss.ligne = n_l;
	bss.decalage=dec; 
	bss.valeur = valorem;                                 
	COLLEC_BSS *pm = calloc(1, sizeof(  *pm )) ;
	COLLEC_BSS* pm2;
	if(pm == NULL) {
		printf("erreur allocation ajout tete\n");
		return NULL;
	}
		
	else if(C == NULL){	
		pm->val = bss;	
		pm->suiv = NULL;
		C = pm;
	}	
	else{
		pm->val = bss ;
		pm->suiv = NULL;
		pm2 = C;
		while (pm2->suiv!= NULL){
            		pm2 = pm2->suiv;
		}	
		pm2->suiv = pm;	
	}
	return C;
}

void afficher_collec_bss(COLLEC_BSS* C){
	COLLEC_BSS* pm=C;	
	while(pm !=NULL){	
		printf("%s \n",pm->val.chaine);
		printf("nbr_operande=%hhu \n",pm->val.NB_op);			
		printf("ligne=%d \n",pm->val.ligne);
		printf("decalage=%d  \n",pm->val.decalage);
		printf("valeur=%d \n",pm->val.valeur); 
		printf("\n");
		pm = pm->suiv;
	}
}
