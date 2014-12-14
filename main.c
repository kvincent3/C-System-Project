#include <stdio.h>
#include "ManageFile.h"
#include <stdlib.h>
#include  <signal.h>
#include "boucle.h"
#include <sys/stat.h>
#include <unistd.h>
#include "commande.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
 ClearScreen();
  printf("-----------------------> WELCOME TO THE SHELL GAME <--------------------------\n\n");
  char *arg=argv[1];
  SaveCommand *saveCo=(SaveCommand *) malloc(sizeof(SaveCommand)); 
  gestionFichier(arg);
  ReadMetaFile(saveCo);// on lit le fichier meta
  printf("\n");
  saveCo->chemin=(char *) malloc(sizeof(char)*100);
  strcat(saveCo->chemin,"temporary");
  do
  {
    saveCo->resultat=-1;
    ManageCommand(saveCo);
  }
      while (saveCo->resultat!=0) ;
  free(saveCo->SaveAllowCommand);
  free(saveCo);
  Delete();
  return 0;
}


