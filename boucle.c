#include <stdlib.h>
#include <stdio.h>
#include  <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "ManageFile.h"
#include "boucle.h"
#include "commande.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void capture (int sig) 
{ 
if (pid >0){
printf("The program was interrupted\n");
kill(pid,SIGUSR1);
}
}

void ManageCommand(SaveCommand *s)
{
  char *line= NULL;
  s->getRedirection=0;
  size_t len = 0;
  ssize_t readln; 
  printf("/%s $",s->chemin);
  readln=getline(&(s->CommandEnterByUser), &len, stdin);// on lit les commandes de l'utilisateur
  if (readln==-1)
  {
    printf("\n");
    s->resultat=-10;
  }
  ManageDirection(s);// gère les redirections
  int fd[2];
  s->echecCommande=0;
  s->tapeCd=0;
  ManageCd(s);
  TapeExit(s);
  int status;
  if(TapePwd(s)==-1)
  {
    if (s->tapeCd==0){
     pipe(fd);
     if ((pid=fork())==0)
         {
		  chdir(s->chemin);
		  close(fd[0]);
		    if (s->getRedirection==0)
		    {
		      dup2(fd[1],1);
		    }
		    else if(s->getRedirection==1)
		    {
			int ret;
			ret = open(s->SaveNameFileRedirect, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		 	dup2(ret,1);
		    }

		    ManageAllCommand(s);
	 	    exit(20);
	  }
	  else
	  { 

   		    signal (SIGINT, capture);
		    waitpid(pid,&status,0);
		    close (fd[1]);
		    char reading_buf[1];
		    int i=0;
                   char out[100000];
		   if(status>0 && *s->CommandEnterByUser !='\n' && s->echecCommande==0 && s->tapeCd==0)
		   {
		       printf("sorry but this game doesn't know this command\n");
		    }
		    while(read(fd[0], reading_buf, 1) > 0)
		    {
 			write(1, reading_buf, 1); // 1 -> stdout0
			close(fd[1]);
                        out[i]=reading_buf[0];
			i++;
	      	    }
		    out[i]='\0';
		    close(fd[0]);
		    //close(fd[1]);
		    s->resultat=strcmp(s->SaveSentence,out);
    	           free(s->CommandEnterByUser);
		   //free(out);
	  }

      }

   }
 //If(FindAnswer)
}

