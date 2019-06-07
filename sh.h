/*Etienne PENAULT 17805598*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <assert.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,    // nbre max de repertoire dans PATH
  MaxPathLength = 512,    // longueur max d'un nom de fichier
};


void decouper(char *, char *, char **, int);//permet de découper un ligne en morceaux selon on séparateur donné.
void recupHost(char *,int);//récupère le nom de la machine
char * getChemin(char *, char **);	//récupère le dossier courant
int lanceurdefonction(char**, char *[], char *, int); //Vérifie les fonctions d'une ligne et les executes.


