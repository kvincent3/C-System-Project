CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
leash : main.o ManageFile.o commande.o boucle.o
	$(CC) $(CFLAGS) *.o -o leash
main.o : main.c ManageFile.h commande.h boucle.h
	$(CC) -c main.c
ManageFile.o : ManageFile.c ManageFile.h
	$(CC) -c ManageFile.c
commande.o : commande.c commande.h
	$(CC) -c commande.c
boucle.o : boucle.c boucle.h
	$(CC) -c boucle.c
