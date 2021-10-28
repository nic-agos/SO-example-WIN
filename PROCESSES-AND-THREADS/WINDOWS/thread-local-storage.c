
#include <windows.h>
#include <stdio.h>

#define EXPOSE  //la define è attiva

#ifdef EXPOSE
int * v[2];  
#endif

__declspec(thread) int  variable;  //variabile è un intero non globale ma TLS, per ogni thread attivo esiste una istanza di variable, incluso il thread main

void ChildThread(char * me)  //ricevo la codifica della mia identità nella variabile me
{

	printf("Child thread %d active - variable is %d\n", (int)me, variable); //re-casting a intero di me che era stato passato come (LPVOID)
	fflush(stdout);
	
#ifdef EXPOSE
	variable = (int)me;
	v[(int)me] = &variable;  //aggiorno l'array
#endif
	Sleep(3000);
	ExitThread((DWORD)me);
}

int main(int argc, char** argv)
{
	HANDLE hThread[2];  //array di 2 handle
	int i;
	DWORD hid;

	variable = 1;

	for (i = 0; i < 2; i++)
	{

		hThread[i] = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)ChildThread,
			(LPVOID)i,  //passa a ChildThread il valore di i (me), il casting perchè posso passare solo puntatori
			NORMAL_PRIORITY_CLASS,
			&hid);

		if (hThread[i] == NULL)
		{
			printf("cannot create thread %d\n", i);
			ExitProcess(-1);
		}

	}

#ifdef EXPOSE
	Sleep(1000);
	for (i = 0; i < 2; i++)
	{
		printf("thread %d - variable value is %d\n", i, *v[i]);  //stampo l'intero presente in TLS
	}
#endif

	for (i = 0; i < 2; i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
	}

}

