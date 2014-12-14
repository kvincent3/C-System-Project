
int verification(char *path);// verifier la permission du dossier
void ManageDirection(SaveCommand *s);// gérer les redirections
void ManageAllCommand(SaveCommand *s);// va pouvoir exécuter toute les commandes
int TypeCd(SaveCommand *s);//l'utilisateur tape Cd
void ManageCd(SaveCommand *s);
int directoryExist(char* directory_name);
int ReturnPlaceCharacter(char *mot);
void TapeExit(SaveCommand *s);
int TapePwd(SaveCommand *s);
int AuthorizeCommand(char *,SaveCommand *s);
char *RecupString(char *source);
void ClearScreen();
void Delete();
void cap(int sig);
int *echecCommande;

