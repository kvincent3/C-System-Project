typedef struct SaveCommand SaveCommand;
struct SaveCommand
{
  char **SaveAllowCommand;// enregistre les commandes autorisées par le fichier meta
  char *SaveSentence;//enregistrer la phrases qui doit être trouvé
  char *CommandEnterByUser;//enregistrer les commandes de l'utilisateur
  char **Arguments;//enregistre les arguments de la commande
  int  increment;// permet de faire des incrémentations
  char *SaveNameFileRedirect;//sauvegarder le nom du fichier pour faire la redirection
  int getRedirection;// entier qui informe si il a eu une redirection
  char *chemin;
  int resultat;
  int tapeCd;
  int echecCommande;
};

void ReadMetaFile(SaveCommand *);
void insertAllowCommand(char,char *,SaveCommand *);
void gestionFichier(char *);
void createFolder ();
void copyFilesFromtar(char *);
int countCommand();
