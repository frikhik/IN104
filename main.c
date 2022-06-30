#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Pour la beauté de l'affichage on ajoute des couleures
#define CF_BLK "\x1B[40m"	//les CF sont pour la couleur de fond
#define CF_GRN "\x1B[42m"
#define CF_YEL "\x1B[43m"
#define CF_HBLK "\x1B[0;100m"

#define CT_GRN "\x1B[0;32m"	//les CT sont pour la couleur de texte
#define CT_RED "\x1B[0;31m"
#define CT_YEL "\x1B[0;33m"

#define RESET "\x1B[0m" 	//reset remet la couleur de texte/fond à ce qui est normal



char** dico(char* fname,int WORDS_SIZE,int *SIZE_DIC);
int jeu(char** WORDS, char* WORD, int SIZE_DIC,char** SOL,int SIZE_SOL);
int hasWord(char** WORDS, char* input, int SIZE_DIC);
void printLigne(char* word, char* seq,char** SOL,int SIZE_SOL);
char* getSeq(char* WORD, char* input);
char* getCouleur(int n);

int NB_MOTS;
int SIZE_DIC;
int SIZE_SOL;
int WORDS_SIZE;
int MAX_ATTEMPTS;
char langue;
char* fname;


int main() {
										//Selectioner la langue
  system("clear");			//vider le terminal(pour la beauté de l'affichage)
  printf("Dans quelle %slangue%s voulez vous jouer ,francais(f) ou anglais(a)): ",CT_GRN,RESET);
  while(langue!='f' && langue!='a'){
  scanf("%s",&langue);
  if(langue=='f'){
  	fname="francais_sans_accents.txt";
  					//nombre de mots dans le dico
	NB_MOTS = 23500;} 		// il faut mettre plus que le vrai nombre sinon certains mots ne sont pas pris en compte, 23000 n'est pas suffisant alors qu'il y en a 22740...
  else if(langue=='a'){
  	fname="wordsword.txt";
	NB_MOTS = 70000;}
  else{printf("Veuillez taper 'a' ou 'f': ");}
  }
	
										//Selectioner le nbr de lettres
  printf("Nombre de %slettres%s dans le mot: ",CT_GRN,RESET);
  scanf("%d",&WORDS_SIZE);
  
  char** WORDS=dico(fname,WORDS_SIZE,&SIZE_DIC);
  int nb_rand;
  srand(time(NULL));
  nb_rand=rand()%(SIZE_DIC+1);
  char* WORD=WORDS[nb_rand];      							
  char** SOL=dico(fname,WORDS_SIZE,&SIZE_SOL);				//initialiser la liste de solutions
								//Nbr d'essais
  printf("Nombre d'%sessais%s: ",CT_GRN,RESET);
  scanf("%d",&MAX_ATTEMPTS);
  int i = 0;
  int res = 0;
  //printf("%s",WORD);                                         //affiche la réponse en début de jeu
 
  
  while (i < MAX_ATTEMPTS + 1) {

    if (i == MAX_ATTEMPTS) {
      printf("%sPerdu, le mot était '%s'.  :(%s\n", CT_RED, WORD, RESET);
      break;
    }
    printf("%sWordle%s | %sessai: %d/%d%s -> ", CT_GRN, RESET, CT_YEL, (i+1), MAX_ATTEMPTS, RESET);
    //printf("%d ",SIZE_SOL);
    res = jeu(WORDS, WORD, SIZE_DIC,SOL,SIZE_SOL);
    //printf("%d ",SIZE_SOL);
    if (res == 1) {
      i++;}
    else if (res == 2) {
      printf("%sBravo! C'est gagné !%s\n", CT_GRN, RESET);
      break;}
    }
 free(WORDS);
 free(SOL);
  return 0;
}


char** dico(char* fname,int NB_LETTRES, int *SIZE_DIC){
    FILE *in=fopen(fname,"rb");
    char str[20]; 						//20 pour eviter le "stack smashing"
    char** tab=malloc(NB_MOTS*sizeof(char*));
    *SIZE_DIC=0;
    fscanf (in,"%s",str);					 //on prend le premier mot du dico
    
    for(int i=0;i<NB_MOTS;i++){
       if (strlen(str)==NB_LETTRES){ 				//si le mot fair le bon nbr de lettres on l'ajoute a la liste de mots
            tab[(*SIZE_DIC)] = malloc (strlen (str) * sizeof (char)) ;
            strcpy(tab[(*SIZE_DIC)],str);
            (*SIZE_DIC) ++;} 					// on compte en meme temps la taille de cette nouvelle liste, utile plus tard
            
       fscanf (in,"%s",str);}					 // si le mot ne fait pas la bonne taille, ou si on a deja utilisé le mot, on passe au prochain
    return tab;}




int jeu(char** WORDS, char* WORD, int SIZE_DIC,char** SOL,int SIZE_SOL) {

  char input[20]; 			// 20 pour eviter le "stack smashing" si le joueur met un mot trop long (ici si le joueur met un mot de plus de 20 lettres, ce sera un probleme)
  int res = 0;

  scanf("%s", input);				//demander un mot au joueur

  int len = (int)strlen(input);
//printf("%d ",SIZE_SOL);
  if (len != WORDS_SIZE) { 			//verifier que le mot fait la bonne taille
    printf("Vous devez jouer un mot de %d lettres, et non %d. Veuillez réessayer:\n", WORDS_SIZE,len);
    }else{
    if (hasWord(WORDS, input, SIZE_DIC)) {	//verifier que le mot est dans la liste de mots de bonne taille
      if (strcmp(WORD, input) == 0) {		//si le mot est juste(le joueur gagne)
        printLigne(WORD, input,SOL,SIZE_SOL);		//affichage
        res = 2;}
      else {					//si le mot n'est pas le bon
        printLigne(WORD, input,SOL,SIZE_SOL);
        res = 1;}}
    else {					// le mot n'est pas dans le dico
      printf("Ce mot n'est pas dans mon dictionaire. Veuillez réessayer\n");
    }}
    //printf("%d ",SIZE_SOL);
  return res;
}

int hasWord(char** WORDS, char* input, int SIZE_DIC) { 		//verifier que le mot est dans la liste de mots de bonne taille

  int res = 0;
  for(int i = 0;i < SIZE_DIC;i++) { //on verifie chaque mot de la liste
    if (strcmp(WORDS[i], input) == 0) {
      res = 1;
      break;
    }}
  return res;}


/////////////////////////////////////////////////////////////////////////////////////////
char** solutions(char** SOL,int *SIZE_SOL,char* input,char* res){ //donne la liste de solutions possibles

int size_sol=0;
char** tab=malloc((*SIZE_SOL)*sizeof(char*));
for(int i = 0;i < *SIZE_SOL;i++) {
	int test=0;
	for(int j=0; j<WORDS_SIZE;j++){
		char a = '0';
		char b = '0';
		a=res[j]-48;				//la variable est transformée en int, avec un décallage de 48
		b=getSeq(SOL[i],input)[j]-48;
		
		if (a==2 && b==2){test+=1;} //les bons mots ont la bonne lettre au mm endroit
		else if (a==0 && !strchr(SOL[i],input[j])){ test+=1;} //Les bons mots ne possedent pas les lettres grises
		else if (a==1 && strchr(SOL[i],input[j])){ test+=1 ;}//les bons mots possedent quelque part les lettres jaunes
		}
		
		if (test==WORDS_SIZE){
	       	tab[(size_sol)] = malloc (strlen (SOL[i]) * sizeof (char)) ;
              		strcpy(tab[(size_sol)],SOL[i]);
			size_sol+=1;
			//printf("%s ",SOL[i]);
			}}
*SIZE_SOL=size_sol;
return tab;}
///////////////////////////////////////////////////////////////////////////////////////////



void printLigne(char* word, char* input,char** SOL, int SIZE_SOL) {  				//Compare le mot et l'input, et affiche le mot avec le code couleur pour chaque lettre

  char* seq = getSeq(word, input);
  //printf("%d ",SIZE_SOL);
  SOL=solutions(SOL,&SIZE_SOL,input,seq);
  //printf("%s\n",SOL[0]);
  //printf("%d ",SIZE_SOL);
  char c = '0';
  int num = 0;
  printf("\n");

  for(int i = 0;i < 3;i++) {  					//Pour que ce soit joli, on va afficher 3 lignes (pour avoir un fond coloré CARRE pour chaque lettre, au lieu d'un RECTANGLE
    for(int j = 0;j < WORDS_SIZE;j++) {				 //Pour chaque lettre du mot
      c = seq[j]; 							//c= chiffre associé a la lettre en position j 
      num = atoi(&c);

      if (i == 1) { 								//Sur la ligne du milieu (la deuxieme), on écrit la lettre
     						 //Pour afficher le texte ou l'arriere plan en couleur, on utilise %s associé au code couleur , avant le texte. RESET remet la couleur normale
        printf(" %s   %c   %s ", getCouleur(num), toupper(input[j]), RESET); //toupper pour avoir la lettre en majuscule, pour la beauté de l'affichage
     } else {
        printf(" %s       %s ", getCouleur(num), RESET); //si on est pas sur la ligne du milieu on met de la couleur seulement
      }}
    printf("\n"); 								//retour a la ligne entre chacune des 3 lignes
  }
  printf("\n");}


char* getSeq(char* word, char* input) { //donne si telle lettre est dans le mot ou non et au bon endroit ou non

  char* res = malloc(sizeof(char) * strlen(word));
  
  for(int i = 0;i < WORDS_SIZE;i++) {
    if ((int)input[i] == (int)word[i]) {res[i] = '2';}				//la lettre est dans le mot et au bon endroit 
    else if (strchr(word, input[i])) {res[i] = '1';}				//la lettre est dans le mot mais pas au bon endroit
    else {res[i] = '0';}				//la lettre n'est pas dans le mot
      }
  return res;}
  

char* getCouleur(int n) { 	//assimile une couleure aux chiffres 0 1 et 2

  char* res ;
  switch(n) {			// "switch" pour essayer d'ecrire if(...) d'une autre facon
    case 0:			//if n=0, ie si la lettre n'est pas dans le mot, couleur grise
      res = CF_HBLK;
      break;
    case 1:			//n=1, lettre dans le mot mais pas au bon endroit, couleur jaune
      res = CF_YEL;
      break;
    case 2:
      res = CF_GRN;
      break;}
      
  return res;}
