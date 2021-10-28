#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


DWORD ThreadFiglio();  //dichiarazione di funzione


int main(int argc, char *argv[]) {

	HANDLE hThread;    //handle del nuovo percorso
	DWORD hid;
	DWORD exit_code;
	
	hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFiglio,
		NULL,
		NORMAL_PRIORITY_CLASS,
		&hid);



	if (hThread == NULL)  printf("Thread creation failure\n");
	else
	{
		WaitForSingleObject(hThread, INFINITE);  //il padre si sincronizza con il figlio, non utilizzo timeout
		GetExitCodeThread(hThread, &exit_code);
		printf("child thread exited with code %d\n", exit_code);
		fflush(stdout);
	}
	printf("parent thread - give me an integer to make me exit:");
	fflush(stdout);

	scanf("%d", &exit_code);  
	
	//quando il main termina lo starter riprende il controllo e lui andrà a chiamare la exitProcess per terminare il main
}



DWORD ThreadFiglio()    //i nomi delle funzioni sono espressioni puntatore alla zona testo dove è presente 
{

	int x;
	printf("child thread - give me an integer to make me exit:");
	fflush(stdout);
	scanf("%d", &x);
	ExitThread(x);
}

