/*Etienne PENAULT 17805598*/

# include "sh.h"

void
decouper(char * ligne, char * separ, char * mot[], int maxmot){
  int i;

  mot[0] = strtok(ligne, separ);
  for(i = 1; mot[i - 1] != 0; i++){
    if (i == maxmot){
      fprintf(stderr, "Erreur dans la fonction decouper: trop de mots\n");
      mot[i - 1] = 0;
    }
    mot[i] = strtok(NULL, separ);
  }
}

int lanceurdefonction(char** mot, char * dirs[MaxDirs], char * cd, int pid){
  int i, tmp, tmp2, etat;
  char pathname[MaxPathLength];

  if (mot[0] == 0)// ligne vide
      return 2;

  //&&//
  //si la variable "pid" (flag) vaut 0 on le fork avant de lancer l'exec sinon il le passe
  //permet de savoir si la fonction est lancée par un enfant ou non
  //si c'est deja un enfant, on creer un enfant d'enfant
    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"&&")==0){
        mot[i]=0;
        tmp2 = fork();
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          waitpid(tmp2,&etat,0);
          if(etat==0){
            tmp2=fork();
            if(tmp2==0){
              lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
              exit(1);
            }
            else{
              wait(0);
              return 2;
            }
          }
          else{
            return 2;
          }
        }
      }
    }

    /*IDEM QUE LE && SANS WAITPID()*/
    for(i=0; mot[i] != NULL; i++){ 
      if(strcmp(mot[i],"&")==0){
        mot[i]=0;
        tmp2 = fork();
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          tmp2=fork();
          if(tmp2==0){
            lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
            exit(1);
          }
          else{
            wait(0);
            return 2;
          }
        }
      }
    }

    /*SI CD, ALORS ON CHANGE DE REPERTOIR*/
    if(strcmp("cd", mot[0]) == 0){
      chdir(mot[1]);
      return 3;
    }

    if(strcmp("creator", mot[0]) == 0 || strcmp("author", mot[0]) == 0){
      printf("\033[31;1mEtienne PENAULT 17805598\033[0m\n");
      return 2;
    }

    if(strcmp("man", mot[0]) == 0 && strcmp("mini", mot[1]) == 0){
    	printf("\033[31;1mFonctionnalitées disponibles:\n-Affichage du répertoire courant devant notre prompt\n-Affichage du nom de l'utilisateur et de la machine devant le prompt\n-\";\" pour marquer la fin d'une commande\n-\"cd\" pour changer de répertoire\n-\"&&\" pour lancer une commande si la précédente a réussi\n-\"&\" pour lancer une commande en arrière plan\n-Les redirections (\">\", \">>\", \"&>\", \"2>\", \"2>>\", \"2>&1\")\n-\"|\" qui gère seulement ce qui le précéde\n-\"author\" ou \"creator\" pour l'identité du réalisateur du projet\n-\"man mini\" pour le manuel\n-\"exit\" pour sortir du Mini-Shell\n\033[0m");
       	return 8;
    }

    /*SI EXIT, ALORS ON RETOURNE 10, PUIS ON EXITERA DEPUIS LE MAIN*/
    if(strcmp("exit", mot[0]) == 0){
      return 10;
    }

    /*REDIRECTION, TOUJOURS LE MEME PRINCIPE POUR TOUTES*/
    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],">")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;
        strcpy(str,mot[i]);	//On stock notre nom de fichier dans str

        out = open(str, O_CREAT | O_WRONLY | O_TRUNC,0); //Creer le fichier/Ecriture seule/efface ce qu'il y a dedans.
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier, sinon obligé de chmod dans le shell.
        tmp2 = fork();
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          if(dup2(out, STDOUT_FILENO)!= STDOUT_FILENO) //on redirge la sortie standard dans notre fichier par le biai de notre fd "out".
            printf("Erreur lors de la redirection");//Remplace la sortie par notre fichier
          close(out);
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          tmp2=fork();
          if(tmp2==0){
            lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
            exit(1);
          }
          else{
            wait(0);
            return 2;
          }
        }
      }
    }

    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],">>")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;

        strcpy(str,mot[i]);
        out = open(str, O_CREAT | O_WRONLY | O_APPEND,0); //O_APPEND permet de rajouter des element au fichier sans ecraser
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier
        tmp2 = fork();
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          if(dup2(out, STDOUT_FILENO)!= STDOUT_FILENO)
    	    printf("Erreur lors de la redirection");//Remplace la sortie par notre fichier
          close(out);
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          tmp2=fork();
          if(tmp2==0){
        	lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
            exit(1);
          }
          else{
            wait(0);
            return 2;
          }
        }
      }
    }

    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"&>")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;

        strcpy(str,mot[i]);
        out = open(str, O_CREAT | O_WRONLY | O_TRUNC,0); //O_APPEND permet de rajouter des element au fichier sans ecraser
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier
        tmp2 = fork();
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          if(dup2(out, STDOUT_FILENO)!= STDOUT_FILENO)
            printf("Erreur lors de la redirection");//Remplace la sortie par notre fichier
          if(dup2(out, STDERR_FILENO)!= STDERR_FILENO)
            printf("Erreur lors de la redirection des erreurs");//Remplace la sortie par notre fichier
          close(out);
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{          
          tmp2=fork();
          if(tmp2==0){
        	lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
            exit(1);
          }
          else{
            wait(0);
            return 2;
          }
        }
      }
    }

    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"2>")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;

        strcpy(str,mot[i]);
        out = open(str, O_CREAT | O_WRONLY | O_TRUNC,0); //O_APPEND permet de rajouter des element au fichier sans ecraser
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier
        tmp2 = fork();
        //printf("%d\n", tmp2);

        if(dup2(out, STDERR_FILENO)!= STDERR_FILENO)
          printf("Erreur lors de la redirection des erreurs");//Remplace la sortie par notre fichier
        close(out);
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          lanceurdefonction(mot, dirs, cd, tmp2);
		  exit(1);
        }
        else{
          waitpid(tmp2,&etat,0);
          return 2;
        }
      }
    }

    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"2>>")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;

        strcpy(str,mot[i]);
        out = open(str, O_CREAT | O_WRONLY | O_APPEND,0); //O_APPEND permet de rajouter des element au fichier sans ecraser
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier
        tmp2 = fork();
        //printf("%d\n", tmp2);

        if(dup2(out, STDERR_FILENO)!= STDERR_FILENO)
          printf("Erreur lors de la redirection des erreurs");//Remplace la sortie par notre fichier
        close(out);
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          waitpid(tmp2,&etat,0);
          return 2;
        }
      }
    }

    for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"2>&1")==0){
        char str[200]; //Nom du fichier cible
        int out;
        mot[i] = 0;
        i++;

        strcpy(str,"/dev/tty"); //Path du terminal donc sortie standard
        out = open(str, O_CREAT | O_WRONLY | O_APPEND,0); //O_APPEND permet de rajouter des element au fichier sans ecraser
        chmod(str,S_IRWXU | S_IRWXG | S_IRWXO); //Donne les permission pour lire le fichier
        tmp2 = fork();
        //printf("%d\n", tmp2);

        if(dup2(out, STDOUT_FILENO)!= STDOUT_FILENO)
          printf("Erreur lors de la redirection de la sortie");//Remplace la sortie par notre fichier
        if(dup2(STDERR_FILENO,STDOUT_FILENO)!= STDOUT_FILENO)
          printf("Erreur lors de la redirection des erreurs prout");//Remplace la sortie par notre fichier
        close(out);
        if(tmp2<0){
          perror("tmp2");
        }
        else if(tmp2==0){
          lanceurdefonction(mot, dirs, cd, tmp2);
          exit(1);
        }
        else{
          waitpid(tmp2,&etat,0);
          return 2;
        }
      }
    }
    /*FIN DES REDIRECTIONS*/

    /*GESTION DES PIPES DE FONCTIONNE PAS, PREND SEULEMENT LA COMMANDE AVANT LE '|'*/
  	for(i=0; mot[i] != NULL; i++){
      if(strcmp(mot[i],"|")==0){
        mot[i]=0;
        int in[2],out[2];
        pipe(in);
        pipe(out);
        tmp2 = fork();
        assert(tmp2 >=0);
        if(tmp2>0){
          if(in != NULL)
            close(in[1]);

          if(out != NULL)
            close(out[1]);

        }
        else{
          if(in != NULL){
            //ON DUPLIQUE LE FD ET ON LE REOUVRE EN TANT QUE STDIN=0
            dup2(in[0], 0);
            //ON FERME L'ENTRÉE DU FD EN ENTRÉE
            close(in[0]);
          }
          if(out != NULL){
            //ON DUPLIQUE LE FD ET ON LE REOUVRE EN TANT QUE STDOUT=1
            dup2(out[1], 1);
            //ON FERME LA SORTIE DU FD EN SORTIE
            close(out[1]);
            lanceurdefonction(&mot[i+1], dirs, cd, tmp2);
          }
        }
      }
    }

    if(pid>0){
      tmp = fork();// lancer le processus enfant
      if (tmp < 0){
        perror("fork");
        return 2;
      }


      if (tmp != 0){// parent : attendre la fin de l'enfant
        while(wait(0) != tmp)
          ;
        return 2;
      }
    }

    // enfant : exec du programme
    for(i = 0; dirs[i] != 0; i++){
      snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
      execv(pathname, mot);
    }
                                // aucun exec n'a fonctionne
    fprintf(stderr, "%s: not found\n", mot[0]);
    exit(1);
}