// this program traps the CTRL+C system event
//intercettimao l'evento ctrl+c da shell, utilizziamo il polling virtuale

//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tchar.h"

char buff[1024];  //accessibile a qualsiasi thread attivo nell'applicazione, anche al thread che viene lanciato per il polling virtuale

int one_shot = 0;

/* se imposto one_shot = 0 vuol dire che il ciclo while continua all'infinito, se durante la sua esecuzione 
 vengono ricevuti altri eventi CTRL+C l'applicazione non li rileva perchè il gestore non ritorna*/ 

// non testo il codice dell'evento
int  myhandler(int event){

	printf("received CTRL+C\n");
	fflush(stdout);
	while (1) {
		printf("%s\n",buff);
		if (one_shot) break; // il ciclo while viene eseguito una sola volta se one_shot = 1
		Sleep(1000);
	};
	return  1; //se non ritornassimo un valore (1 = true) non riusciremo a dire al kernel che la gestione dell'evento è avvenuta
}


int main(int argc, _TCHAR* argv[])
{
	BOOL result;
	/*non specifichiamo per quale evento impostiamo il gestore, lo aggiungiamo
	semplicemente alla pila dato che questa sarà composta da un solo elemento */
	result = SetConsoleCtrlHandler((PHANDLER_ROUTINE)myhandler, 1); //1 corrisponde a true quindi stiamo aggiungendo alla pila l'handler (gestore)
	printf("handler posting returned %d \n", result);
	fflush(stdout);

	while (1){
		scanf("%s", buff);
		if (strcmp(buff, "quit") == 0) break;
	}
	return 0;
}


