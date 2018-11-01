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
#include <liste.h>

enum{INITS,TEXTS,DATAS,BSSS};
enum{INI,REG,OFFSET_DECI,OFFSET_HEXA,OFFSET,DECI,HEXA,ETIQUETTE, VIRGULE};

/*
int HexaDec(char* chaine)  /// Transforme une chaine de caractère d'Hexa en Décimal
{
	int dec = 0;
	int n = strlen(chaine) -2; ///on ne compte pas le 0x du début
	int i = 2; ///on commence à regarder le caractère après le x
	while(n!=0)
	{
		if (isdigt(chaine[i])) dec = dec + (atoi(chaine[i])-48)*pow(16,n); ///atoi convertit un caractère ascii en sont équivalent dec, pow met à la puissance, -48 parce que 0 en ascii vaut 48
		else dec = dec + (atoi(toupper(chaine[i]))-55)*pow(16,n); ///de même, toupper met la minuscule en majuscule, -55 parce que A doit valloir 10 et que en ascci A vaut 65
		i++;
		n--;
	}
	printf("dec = %d\n",dec);
	return dec;
}
*/

LISTE_LEXEME* Instruction(LISTE_LEXEME* Liste, COLLEC_TEXT* Collec_instruc, int NIsnt, DICO* dictionary)/*ajoute une migne en vérifiant que les instructions sont bonnes et bien formulées*/
{
	char* chaine;
	int NB_op;
	int i ;
	int counter = 1;
	int S = INI;
	printf("42\n");
	if ( (i = ChercInst(dictionary, Liste->val.chaine, NIsnt))==-1) ERROR_MSG("Erreur Instruction non valide ligne %d : \"%s\"\n",Liste->val.ligne, Liste->val.chaine);
	else
	{	
		printf("46\n");
		chaine = Liste->val.chaine;
		NB_op = dictionary[i].nb_ope;
		printf("NB_op : %d\n", NB_op);
		/*traiter le décalage*/
		Liste = Liste->suiv;
		while(strcmp(Liste->val.chaine,"\n")!=0)
		{
			switch (S)
			{
				case INI:
					puts("INI");
					if (Liste->val.identifiant == 1) {counter--; Liste = Liste->suiv;}/* si il y a un commentaire, ce n'est pas une opérande donc le compteur diminue de 1*/
					else if (Liste->val.identifiant == 3) S = ETIQUETTE;
					else if (Liste->val.identifiant == 6) S = OFFSET_DECI;
					else if (Liste->val.identifiant == 8) S = REG;
					else if (Liste->val.identifiant == 9) S = HEXA;
					else ERROR_MSG("Erreur oppérande invalide ou virgule mal placée ligne %d \n", Liste->val.ligne);
					break;
				case REG:
					puts("REG");
					if (OpAttendue(dictionary,i,S,counter, Liste)==1){/*EcritureOP(counter,&instru, Liste);*/ S = VIRGULE; Liste = Liste->suiv; counter++;}/* fonction de mathis empiler */
					break;
				case OFFSET_DECI:
					puts("OFFSET_DECI");
					if(Liste->suiv->val.identifiant == 10) S = OFFSET;
					else S = DECI;
					break;
				case OFFSET_HEXA:
					puts("OFFSET_HEXA");
					if(Liste->suiv->val.identifiant == 10) S = OFFSET;
					else S = HEXA;
					break;
				case DECI:
					puts("DECI");
					if (OpAttendue(dictionary,i,S,counter,Liste)==1) {/*EcritureOP(counter,&instru, Liste);*/ S = VIRGULE; Liste = Liste->suiv; counter++;}
					break;
				case HEXA:
				puts("HEXA");
					if (OpAttendue(dictionary,i,S,counter,Liste)==1) {/*EcritureOP(counter,&instru, Liste)*/; S = VIRGULE; Liste = Liste->suiv; counter++;}
					break;
				case OFFSET:
					puts("OFFSET");
					if (OpAttendue(dictionary,i,S,counter,Liste)==1) 
					{
						/*EcritureOP(counter,&instru, Liste);*/
						if (Liste->suiv->suiv->val.identifiant == 8 && Liste->suiv->suiv->suiv->val.identifiant == 11)
						{
						counter++;
						Liste = Liste->suiv->suiv->suiv->suiv;
						S = VIRGULE;
						}
						else ERROR_MSG("Erreur il faut une parentèse ou une vigule ligne %d\n",	Liste->val.ligne);
					}	
					break;
				case ETIQUETTE:
					puts("ETIQUETTE");
					if (OpAttendue(dictionary,i,S,counter, Liste)==1){/*EcritureOP(counter,&instru, Liste);*/ S = VIRGULE; Liste = Liste->suiv; counter++;}/* fonction de mathis empiler */
					/*appeler la fonction etiquette, et checker si elle est valide comme opérande */
					/*S = VIRGULE;
					Liste = Liste->suiv;
					counter++;*/
					break;
				case VIRGULE:
					puts("VIRGULE");
					if (Liste->val.identifiant == 7 && counter%2==0 && strcmp(Liste->val.chaine,"\n")!=0) {S = INI; Liste = Liste->suiv; counter++;} /* c'est une virgule, on */
					 	/*vérifie si elle est bien en positionné après une oppérande donc counter pair et si elle n'est pas en fin de ligne*/
					else ERROR_MSG("Erreur il manque peut etre une virgule ligne %d\n", Liste->val.ligne);
					break;
			}
		}
	}
	counter--; /* on enlève le retour ligne qu'on a compté, il faut maintenant gérer les virgules*/
	if (counter >1) counter = ((counter-1)/2)+1;
	printf("counter : %d\n", counter);
	if (counter == dictionary[i].nb_ope) printf(" nombre d'opérandes valide\n");
	else ERROR_MSG("Erreur nombre d'opérandes invalide ligne %d\n", Liste->val.ligne);
	/*fonction de mathis pour empiler sur collec*/
	return Liste;
}
					
void EcritureOP(int counter, TEXT* instru, LISTE_LEXEME* Liste)
{
	if (counter == 1) printf("ecriture ope 1\n");			/*fonction mathis*/
	else if (counter == 3) printf("ecriture ope 2\n");		/*fonction mathis*/
	else if (counter == 5) printf("ecriture ope 3\n");		/*fonction mathis*/
	else ERROR_MSG("Erreur nombre d'oppérandes invalides ligne %d \n",Liste->val.ligne);
}
		
int OpAttendue(DICO* dictionary, int i, int S, int counter, LISTE_LEXEME* Liste)
{
	printf("i : %d, S : %d, counter : %d\n",i, S, counter);
	if ( counter == 1)
	{
		printf("dictionary[%d].type1 : %s\n",i,dictionary[i].type1);
		if (S == REG && strcmp(dictionary[i].type1, "R")== 0) return 1;
		else if(S == HEXA && strcmp(dictionary[i].type1, "H")==0) return 1;
		else if(S == DECI && strcmp(dictionary[i].type1, "D")==0) return 1;
		else if((S == OFFSET || S == ETIQUETTE ) && strcmp(dictionary[i].type1, "O")==0) return 1;
		else ERROR_MSG("Erreur oppérande invalide ligne %d \n",Liste->val.ligne);
	}
	else if (counter == 3)
	{
		if (S == REG && strcmp(dictionary[i].type2, "R")==0) return 1;
		else if(S == HEXA && strcmp(dictionary[i].type2, "H")==0) return 1;
		else if(S == DECI && strcmp(dictionary[i].type2, "D")==0) return 1;
		else if((S == OFFSET || S == ETIQUETTE ) && strcmp(dictionary[i].type2, "O")==0) return 1;
		else ERROR_MSG("Erreur oppérande invalide ligne %d \n",Liste->val.ligne);
	}
	else if (counter == 5)
	{
		if (S == REG && strcmp(dictionary[i].type3, "R")==0) return 1;
		else if(S == HEXA && strcmp(dictionary[i].type3, "H")==0) return 1;
		else if(S == DECI && strcmp(dictionary[i].type3, "D")==0) return 1;
		else if((S == OFFSET || S == ETIQUETTE ) && strcmp(dictionary[i].type3, "O")==0) return 1;
		else ERROR_MSG("Erreur oppérande invalide ligne %d \n",Liste->val.ligne);
	}
	else ERROR_MSG("Erreur il y a trop d'opérande ligne %d\n",Liste->val.ligne);
	return 0;
}

int ChercInst(DICO* dictionary, char* chaine, int N) /*fonction qui prend en argument le dictonnaire des instruction et une chaine de caractère et retourne l'addresse de cette chaine dans le tableau */
{								/*si c'est une instruction, Null sinon*/
	int i = 0;
	while (i<N-1)/*-1 sinon core dumpted*/
	{
		printf("dictionary[%d] : %s\n",i, dictionary[i].inst);
		if (strcmp(dictionary[i].inst, chaine)==0) 
		{
			printf("i = %d\n",i);
			return i;
		}
		i++;
	}
	printf("i = %d\n",i);
	return -1;
}

DICO* Dico(FILE* file, int* pNInst)
{
	int i = 0;
	fscanf(file,"%d",pNInst);
	printf("NInst : %d\n",*pNInst);
	DICO* tab = NULL;
	DICO instru;
	char inst[5];
	int nb_ope = -1;
	tab = calloc(*pNInst, sizeof(DICO));
	if (tab==NULL) ERROR_MSG("Erreur allocation tab\n");
	while(i<=*pNInst)
	{
		fscanf(file,"%s",inst);
		fscanf(file,"%d",&nb_ope);
		printf("inst : %s, nb_ope : %d\n", inst, nb_ope);
		if (nb_ope==3)
		{
			fscanf(file,"%s",instru.type1);
			fscanf(file,"%s",instru.type2);
			fscanf(file,"%s",instru.type3);
			strcpy(instru.inst, inst);
			instru.nb_ope = nb_ope;
			printf("instru : \n");
			printf("%s\n%d\n%s\n%s\n%s\n",inst,nb_ope,instru.type1,instru.type2,instru.type3);
			tab[i] = instru;
		}
		else if (nb_ope==2)
		{
			fscanf(file,"%s",instru.type1);
			fscanf(file,"%s",instru.type2);
			strcpy(instru.inst, inst);
			instru.nb_ope = nb_ope;
			printf("instru : \n");
			printf("%s\n%d\n%s\n%s\n%s\n",inst,nb_ope,instru.type1,instru.type2,instru.type3);
			tab[i] = instru;
		}
		else if (nb_ope==1)
		{
			fscanf(file,"%s",instru.type1);
			strcpy(instru.inst, inst);
			instru.nb_ope = nb_ope;
			printf("instru : \n");
			printf("%s\n%d\n%s\n%s\n%s\n",inst,nb_ope,instru.type1,instru.type2,instru.type3);
			tab[i] = instru;
		}
		else if (nb_ope==0)
		{
			strcpy(instru.inst, inst);
			instru.nb_ope = nb_ope;
			printf("instru : \n");
			printf("%s\n%d\n%s\n%s\n%s\n",inst,nb_ope,instru.type1,instru.type2,instru.type3);
			tab[i] = instru;
		}
		else ERROR_MSG("Erreur nombre d'instruction invalide dans le dictinnaire\n");
		i++;
	}
	printf("Création du dictionnaire réussi !\n");
	return tab;
}	

void Gram(LISTE_LEXEME* Liste)
{
	
	LISTE_LEXEME* Lcopie = Liste; 	/*pas sur que ca serve*/
	COLLEC_TEXT* Collec_instruc = NULL;
	int S = INITS;
	int NInst = 0;
	FILE        *file   = NULL;
	DICO* dictionary;
	file = fopen( "doc/dico_fonctions.txt", "r" );
	if ( NULL == file ) {
        	/*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        	ERROR_MSG("Error while trying to open \"doc/dictionnaire.txt\" file --- Aborts");
    	}
    	dictionary = Dico(file,&NInst);
    	printf("229 dictionary = %s\n", dictionary[0].inst);
    	printf("230 dictionary = %s\n", dictionary[1].inst);
    	printf("229 dictionary = %s\n", dictionary[2].inst);
    	printf("230 dictionary = %s\n", dictionary[3].inst);
    	fclose(file); /*le tableau est créé on peut fermer le fichier*/
	while(Lcopie != NULL)	
	/*ouverture du fichier symboles et Tableux de symbole, fermet le fichier*/
	{
		switch(S)
		{
			case INITS:/*prendre en compte les retour ligne et le commentaire*/
				puts("INITS");
				if(strcmp(Lcopie->val.chaine,".text")==0) {S = TEXTS; printf(".text\n");}
				else if (strcmp(Lcopie->val.chaine,".data")==0) {S = DATAS; printf(".data\n");}/* le break fait il sortir du case ou du switch*/
				else if (strcmp(Lcopie->val.chaine,".bss")==0) {S = BSSS; printf(".bss\n");}
				else if (Lcopie->val.identifiant == 1 || strcmp(Lcopie->val.chaine,"\n")==0 || strcmp(Lcopie->val.chaine,":")==0) S = INITS;
				else if (Lcopie->val.identifiant == 3 && Lcopie->suiv->val.identifiant == 4){S = INITS; printf("étiquette\n");}/* c'est une étiquette donc faire appelle à une foction qui prend en argument L'addresse */
				else if (strcmp(Lcopie->val.chaine,".set")==0 && strcmp(Lcopie->suiv->val.chaine,"noreorder")==0) {S = INITS; Lcopie=Lcopie->suiv;printf(".set noreorder\n");}/*de la liste et Lcopie*/
				else ERROR_MSG("Erreur ligne %d présence de \"%s\" non comforme",Lcopie->val.ligne,Lcopie->val.chaine);/*pour qu'il y ait une erreur pour toutes les autres instructions or de*/
				break;													/* .text .data .bss*/
			case TEXTS:
				puts("TEXTS");
				if (strcmp(Lcopie->val.chaine,".data")==0) {S = DATAS; }
				else if (Lcopie->val.identifiant == 3 && Lcopie->suiv->val.identifiant == 4){S = TEXTS;}
				else if (Lcopie->val.identifiant == 3) {S = TEXTS;Lcopie = Instruction(Lcopie, Collec_instruc, NInst, dictionary);}/*lancer une fonction qui prend en argument Lcopie et Linst et retourne Lcopie de la ligne suivante*/
				else if (Lcopie->val.identifiant == 1 || strcmp(Lcopie->val.chaine,"\n")==0 || strcmp(Lcopie->val.chaine,":")==0) S = TEXTS;
				else ERROR_MSG("Erreur ligne %d présence de \"%s\" non comforme",Lcopie->val.ligne,Lcopie->val.chaine);
				break;
			case DATAS:
				puts("DATAS");
				if (strcmp(Lcopie->val.chaine,".bss")==0) S = BSSS;
				else if (Lcopie->val.identifiant == 1 || strcmp(Lcopie->val.chaine,"\n")==0 || strcmp(Lcopie->val.chaine,":")==0) S = DATAS;/*lancer une fonction qui prend en argument Lcopie */
				else if (Lcopie->val.identifiant == 5 ) {S = DATAS;}/*et Ldata et retourne Lcopie de la ligne suivante, pensez à prendre en et qui retourne possiblement Ldata*/
				else if (Lcopie->val.identifiant == 3 && Lcopie->suiv->val.identifiant == 4){S = DATAS;}/* c'est une étiquette donc faire appelle à une foction qui prend en argument L'addresse*/ 
				else ERROR_MSG("Erreur ligne %d présence de \"%s\" non comforme",Lcopie->val.ligne,Lcopie->val.chaine);/*de la table de hachage et Lcopie*//* et qui retourne possiblement Ldata*/
				break;
			case BSSS:
				puts("BSS");
				if (strcmp(Lcopie->val.chaine,".space")==0) {S = BSSS;} /*lancer un fonction qui prend en argument Lcopie et Lbss et retourne Lcopie de la ligne suivant*/
				else if (Lcopie->val.identifiant == 1 || strcmp(Lcopie->val.chaine,"\n")==0 || strcmp(Lcopie->val.chaine,":")==0) S = BSSS;
				else if (Lcopie->val.identifiant == 3){S = DATAS;}/* c'est une étiquette donc faire appelle à une foction qui prend en argument L'addresse de la table de hachage et Lcopie*/
				else ERROR_MSG("Erreur ligne %d présence de \"%s\" non comforme",Lcopie->val.ligne,Lcopie->val.chaine);/* et qui retourne possiblement Ldata*/
				break;
		}
			Lcopie = Lcopie->suiv;
	}
}
