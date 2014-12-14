#include "ManageFile.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void gestionFichier(char *c) //on va récupérer le fichier tar
{

  if(fork()==0) // on creer deux processus un pour creer le dossier namefolder l'autre pour copier le fichier tar
  {
    createFolder();//on crée le dossier
    exit(0);// on termine le processus fils
  }
  else
  {
   waitpid(-1, NULL, 0);// on attend que le dossier soit crée
   copyFilesFromtar(c);//une fois crée on fait les copies 
  }
}

void createFolder()
{ 
  char *nomDossier[]={"mkdir","temporary",NULL};
  execvp("mkdir",nomDossier);
}

void copyFilesFromtar(char *c)
{
  if(fork()==0) // on exécute execvp dans un processus fils
    {
		char *extract[]={"tar","-xf",c,"-C","temporary",NULL};
		execvp("tar",extract);
		exit(1);
    }
    else
   {
	waitpid(-1, NULL, 0);// on attend la fin de l'extraction
   }
}


void ReadMetaFile(SaveCommand *s)// on va ouvrir le fichier meta grâce au nom du dossier
{
   FILE *fichier=NULL;
   fichier=fopen("temporary/meta","r");
   char *line= NULL;
   char *LINE=NULL;
   size_t len = 0;
   ssize_t read;
   if (fichier != NULL)
   {
	s->increment=0;
    s->SaveAllowCommand=(char **)malloc (100*sizeof(char *));
    *(s->SaveAllowCommand)=(char *)malloc(100*sizeof(char ));// on alloue l'espace pour enregistrer les commandes autaurisées
    while ((read =getline(&line, &len, fichier)) != -1) // on lit ligne par ligne
    {
	   LINE=strdup(line);
       char c=*LINE;
	   insertAllowCommand(c,LINE,s);//on insert les commandes autorisées dans la structure
       //printf("%s", line);
    }
        printf("The allowed commands are : \n");
        int h=0;
        while( *((s->SaveAllowCommand)+h) != NULL)
        {
	  printf("--->%s\n",*((s->SaveAllowCommand)+h));
          h++;
	}
   }
  unlink("temporary/meta");// on supprime le fichier meta
}

void insertAllowCommand(char c,char *line,SaveCommand *s)
{
  switch (c)
  {
	case '$':
    *(s->SaveAllowCommand+s->increment)=line+2;
     *(*(s->SaveAllowCommand+s->increment)+strlen(*(s->SaveAllowCommand+s->increment))-1)='\0'; 
	s->increment++;
	break;
	case '>':
	 s->SaveSentence =line+2;
	break;
	case '#':
    break;
  }
}





