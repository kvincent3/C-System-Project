#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ManageFile.h"
#include "commande.h"
#include "string.h"

int verification(char *path)// permet de vérifier les droits d'un dossier
{
  int resultat=0;
  char permis[]={'x','w','r'};
  struct stat buf;
  if(stat(path,&buf)!=-1)// si le fichier existe
  {
	  mode_t mode= buf.st_mode;
	  int i; 
	  char enregistrement[9];
	  char a;
	  for (i=0;i<9;i++)
	  {
		  a=permis[(8-i)%3];
		  if (mode & (0400 >>i))
		  {
		    enregistrement[i]=a;
		  }
		  else
		  {
			enregistrement[i]='-';
		  }

	  }
	  if (enregistrement[2]=='x')
	  {
		return 1;
	  }
  }
  return -1;
}


void ManageDirection(SaveCommand *s)
{

  char *src = strchr(s->CommandEnterByUser,'>');
  if (src != NULL)
  {
	strtok(s->CommandEnterByUser,">");
	s->SaveNameFileRedirect=strtok(NULL,">");
	strtok(s->SaveNameFileRedirect," \n");
	strtok(s->SaveNameFileRedirect," ");
	s->SaveNameFileRedirect=RecupString(s->SaveNameFileRedirect);
	s->getRedirection=1;
  }
}

char *RecupString(char *source)
{
  char *dest= (char *) malloc(sizeof(char)*50);
  int i;
  int p=0;
  for(i=0;i<strlen(source);i++)
  {
    if(source[i] != ' ')
    {
	dest[p]=source[i];
	p++;
    }

  }
    return dest;
  free(dest);
}

void ClearScreen()
{
  if(fork()==0) // on exécute execvp dans un processus fils
    {
		char *extract[]={"clear",NULL};
		execvp("clear",extract);
		exit(3);
    }
    else
   {
	waitpid(-1, NULL, 0);// on attend la fin de l'extraction
   }

}

void ManageAllCommand(SaveCommand *s)
{
    char *src = strchr(strdup(s->CommandEnterByUser),'|');
    char *ok =strtok(s->CommandEnterByUser,"|\n");
    char **SousArguments=(char **) malloc(50*sizeof(char *));
    int v=0;
    for (v=0;v<50;v++)
    {
	*SousArguments=(char *) malloc(50*sizeof(char));
    }
    int i=0;
    while(ok != NULL)
    {
       SousArguments[i]=ok;
       i++;
       ok=strtok(NULL, "|\n");
    }
    int p;
    for (p=0;p<i;p++)
    {
      char **DecomposeArg=(char **) malloc(50*sizeof(char *));
     *DecomposeArg=(char *) malloc(50*sizeof(char));
      char *srcdeux = strchr(strdup(SousArguments[p]),'<');
      char *entre;
      int in=0;
      int ret ;
      if (srcdeux != NULL)
      {
         SousArguments[p]=strtok(SousArguments[p],"<");
         entre=strtok(NULL,"<");
         entre=RecupString(entre);
	 in=1;
      }
      char *okok=strtok(SousArguments[p]," \n");
      int v=0;
      while (okok != NULL)
      {
	DecomposeArg[v]=okok;
	v++;
	okok=strtok(NULL," \n");
      }
     if (AuthorizeCommand(DecomposeArg[0],s)==0)
        {
	      int fd[2];
	      pipe(fd);

	      if (p<i-1)
	      {
		 if(fork()==0)
	  	 {
		    close(fd[0]);
		    if(in==1)
		    {
		       ret = open(entre, O_RDONLY,0);
		       dup2(ret,0);
		    }else{
		    dup2(fd[1],1);}
		    close(fd[1]);
		    execvp(*DecomposeArg,DecomposeArg);
		    exit(0);
		 }
		 close(fd[1]);
		 wait(-1,NULL, 0);
		 dup2(fd[0],0);
	      }
	      else
	      {
		 if(in==1)
		    {
		       ret = open(entre, O_RDONLY,0);
		       dup2(ret,0);
		    }
		execvp(*DecomposeArg,DecomposeArg);
	      }

         }
         else
         {
		int c=0;
		for(c=0;c<50;c++)
		{
		   free(SousArguments[c]);
		}
		free(SousArguments);
                free(DecomposeArg[0]);
                free(DecomposeArg);
		exit(EXIT_FAILURE);
          }

    }

    int c=0;
    for(c=0;c<50;c++)
    {
      free(SousArguments[c]);
    }
    free(SousArguments);
}



int TypeCd(SaveCommand *s)
{
 char *copie=strdup(s->CommandEnterByUser);
 strtok(copie," ");
 if (strcmp(copie,"cd")==0 || strcmp(s->CommandEnterByUser,"cd\n")==0)
 { 
    return 0;
 }
 return -1;
}

void ManageCd(SaveCommand *s)
{
 if (TypeCd(s)==0)
 {
          s->tapeCd=1;
	  strtok(s->CommandEnterByUser," ");
	  char *path=strtok(NULL," ");
	  char *dossier=strtok(path,"\n");
	  if(dossier != NULL)
          {
		  strtok(dossier,"/");
		  char *slash="/";
		  int i=0;
		  int fail =0;
		  char *echec=strdup(s->chemin);
		  char *copie=strdup(s->chemin);
		  char *copiedossier=strdup(dossier);
		  do 
		  {
			if(strcmp(dossier,"temporary")!=0)
			{
		           if(strcmp(dossier,"..")!=0)
			     {
				if(dossier != NULL)
				{
				  strcat(copie,"/");
				  strcat(copie,dossier);
				  if (directoryExist(copie)==0 && verification(copie)==1)
				   {
					 strcat(s->chemin,"/");
					 strcat(s->chemin,dossier);
				    }
				   else
				     {
					printf("Sorry but the folder doesn't exist or you don't have right\n");
					fail=1;
					s->chemin=echec;
					return;
				     }
				}
			     }
			     else if (strcmp(dossier,"..")==0 && strcmp(s->chemin,"temporary")!=0)
			     {
				int slash=ReturnPlaceCharacter(s->chemin);
				*(s->chemin+slash)='\0';
				s->chemin=strdup(s->chemin);
				copie=strdup(s->chemin);
				
			     }
                     }  
		     else 
		     {
			//free(s->chemin);
		        s->chemin=(char *) malloc(sizeof(char)*100);
			strcat(s->chemin,"temporary");
			copie=strdup(s->chemin); 
			copiedossier=strdup(dossier);
                     }
		     dossier=strtok(NULL,"/");
		  }
		  while (dossier!=NULL && fail==0);

      	 }
	 else
         {
	    // free(s->chemin);
	     s->chemin=(char *) malloc(sizeof(char)*100);
	     s->echecCommande=1;
             strcat(s->chemin,"temporary"); 
         }

  }
}

int directoryExist(char* directory_name)
{
    struct stat fstat;                
    if(stat(directory_name, &fstat) ==0){
          if(fstat.st_mode & S_IXUSR) {
              return 0 ;
          }
    }
    return 1;
}


int ReturnPlaceCharacter(char *mot)
{
  int i;
  if ( strrchr(mot,'/') != NULL)
  {
      for (i=strlen(mot)-1;i>0;i--)
      {
	 if (mot[i]=='/')
         {
            return i;
         }
      }
  }
}

void Delete()
{
  if(fork()==0) // on exécute execvp dans un processus fils
    {
		char *extract[]={"rm","-rf","temporary",NULL};
		execvp("rm",extract);
		exit(0);
    }
    else
   {
	waitpid(-1, NULL, 0);// on attend la fin de l'extraction
   }
}
void TapeExit(SaveCommand *s)
{
  if (strcmp(s->CommandEnterByUser,"exit \n")==0 || strcmp(s->CommandEnterByUser,"exit\n")==0 ||  s->resultat==-10)
  {
        free(s->SaveAllowCommand);
        free(s->chemin);
        free(s);
	Delete();
	exit(0);

  }
}

int TapePwd(SaveCommand *s)
{
  if (strcmp(s->CommandEnterByUser,"pwd\n")==0)
  {
	printf("/%s\n",s->chemin);
  }
}

int AuthorizeCommand(char *co,SaveCommand *s)
{
  int i;
  for(i=0;i<100;i++)
  {
     if(strcmp(co,*(s->SaveAllowCommand+i))==0)
     {
      return 0; 
     }
  }
  return -1;
}


