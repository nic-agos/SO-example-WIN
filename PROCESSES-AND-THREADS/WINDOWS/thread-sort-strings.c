
/******************************************************************
this program allows sorting strings coming form the standar input
it uses one thread for input and one thread at a time for sorting 
*******************************************************************/
//mentre il main thread è in wait dell'input non spreco quel tempo ma creo un nuovo thread che in contemporanea lavora sull'ordinamento delle stringhe

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR	1000
#define MAX_STRING      1000


int string_number = 0;
char *string_array[MAX_STRING];
DWORD exit_code;

// riceve in input una stringa alla volta
void OrderThread(char *new_string)
{
	int i, j;

	//printf("Ordering thread active (inserting: %s)\n",new_string);

	for (i = 0; i < string_number; i++)	//scorro le stringhe
	{
		if (strcmp(new_string, string_array[i]) <= 0)  //confronto la stringa da inserire con quelle già presenti, new_string o uguale o precdente a quella analizzata
		{
			for (j = 0; j<(string_number - i); j++) 
			{
				string_array[string_number - j] = string_array[string_number - j - 1]; //scalo le stringhe una posizione avanti a partire dall'ultima andando a ritroso
			}
			break;
		}
	}
	string_array[i] = new_string;
	string_number++;
	exit_code = 0;
	ExitThread(exit_code);
}

int main(int argc, char** argv)
{
	HANDLE hThread;
	DWORD hid;
	int i;
	char *old_buffer;
	DWORD exit_code;
	char buffer[MAX_CHAR];
	int notfirst = 0;

	while (1) 
	{
		printf("Inserisci stringa: ");
		fflush(stdout);
		scanf("%s", buffer);
		if (strcmp(buffer, "quit") == 0)
		{
			break;
		}
		old_buffer = strdup(buffer);	// strdup alloca dinamicamente memoria per ospitare la stringa e la copia quindi duplic
		if (notfirst)  //capisce se la stringa presa in input è la prima o meno, se non è la prima vuol dire che ho già attivato prima un thread
		{
			WaitForSingleObject(hThread, INFINITE);  //aspetto l'esecuzione del thread precedente
			GetExitCodeThread(hThread, &exit_code);
		}
		else
		{
			notfirst = 1;
		}


		// creo il nuovo thread che opera per ordinare l'archivio mentre il main thread chiama una nuova scanf
		hThread = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)OrderThread, //funzione dentro la quale il nuovo thread vivrà
			(LPVOID)old_buffer,  //indirizzo di memoria dove la stringa è salvata (parametro della funzione chiamata)
			NORMAL_PRIORITY_CLASS,
			&hid);

		if (hThread == NULL)
		{
			printf("cannot create thread %d\n");
			ExitProcess(-1);
		}
	}
	// in questa condizione entro soltanto se ho ricevuto il comando quit da riga di comando
	if (notfirst) //aspetto l'ultimo thread se ho inserito quit in input prima di terminare
	{
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &exit_code);
	}
	for (i = 0; i< string_number; i++) printf("String %d: %s\n", i, string_array[i]);
	ExitProcess(0);
}


