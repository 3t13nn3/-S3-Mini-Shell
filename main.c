/*Etienne PENAULT 17805598*/

# include "sh.h"
# define PROMPT "%"

/*RECUPERE L'HOSTNAME (le nom de la machine)*/
void recupHost(char *host,int MaxMot){
  gethostname(host,MaxMot);
}

/*RECUPERE LE DOSSIER COURANT A PARTIR DU CHEMIN*/
char * getChemin(char * cd, char ** mot){
  char * user[MaxMot];
  int i=0;
  if(strcmp(mot[0],"cd") && strcmp(mot[0],"")){
    getcwd(cd, sizeof(cd)); // recupère le chemin courant
    while(user[i+1] != NULL)
      i++;
  }
  else if ((strcmp(mot[0],"cd") == 0 ) || (strcmp(mot[0],"") == 0 )){
    getcwd(cd, sizeof(cd)); // recupère le chemin courant
    decouper(cd, "/", user, MaxMot);

      while(user[i+1] != NULL)
    i++;
  }
  if(user[i] == NULL)
    return "/"; //Cas de la racine du system
  return user[i];
}

int main(int argc, char * argv[]){
  char ligne[MaxLigne];
  char * mot[MaxMot];
  char * dirs[MaxDirs];
  char * tab[MaxMot];
  int i,var;

  char host[MaxMot];
  char cd[MaxMot];
  char user[MaxMot];
  mot[0] = "";  //On déclare le premier mot en tant que "" pour lever le cas particulier du démarrage du minishell avec getChemin();
  
  system("clear");
  printf("\033[34;5;1m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\n");
  printf("\033[34m~\033[32m~\033[34m~ \033[32mM\033[34mI\033[32mN\033[34mI\033[32m-\033[34mS\033[32mH\033[34mE\033[32mL\033[34mL \033[32m~\033[34m~\033[32m~ \033[34mE\033[32mT\033[34mI\033[32mE\033[34mN\033[32mN\033[34mE \033[32mP\033[34mE\033[32mN\033[34mA\033[32mU\033[34mL\033[32mT \033[34m~\033[32m~\033[34m~ \033[32mS\033[34mY\033[32mS\033[34mT\033[32mE\033[34mM\033[32mE \033[34mD\033[32m'\033[34mE\033[32mX\033[34mP\033[32mL\033[34mO\033[32mI\033[34mT\033[32mA\033[34mT\033[32mI\033[34mO\033[32mN \033[34m~\033[32m~\033[34m~\n");
  printf("\033[34;5;1m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[32m~\033[34m~\033[0m\n\n");
  
  recupHost(host, MaxMot); //On récupère le nom de la machine dans "host";
  strcpy(user,getenv("USER")); // Idem pour l'utilisateur dans user

  getcwd(cd, sizeof(cd)); //On récupère le chemin

  //Decouper PATH en repertoires
  decouper(getenv("PATH"), ":", dirs, MaxDirs);

  //Lire et traiter chaque ligne de commande 
  for(printf("\033[32;1m[%s@%s \033[34m%s\033[32m]%s\033[0m ",user,host, getChemin(cd,mot), PROMPT); fgets(ligne, sizeof ligne, stdin) != 0; printf("\033[32;1m[%s@%s \033[34m%s\033[32m]%s\033[0m ",user,host, getChemin(cd,mot), PROMPT)){
    decouper(ligne, ";", tab, MaxMot);//Découpe notre ligne en plusieurs commandes dans tab.
    for(i=0; tab[i] != NULL; i++){  //lis les differentes commandes dans tab.
      decouper(tab[i], " \t\n", mot, MaxMot);

      var=lanceurdefonction(mot,dirs,cd,1); //appel du coeur du programme qui traite les commandes

      if(var==2){//Si tout se passe bien
        continue;
      }
      if(var==3){ //Au cas ou on change de répertoire, on actualise le chemin
        getcwd(cd, sizeof(cd));
        continue;
      }
      if(var==10){ //Quitter
        printf("A la revoyure!\n\n");
        return 0;
      }
    }
  }
}