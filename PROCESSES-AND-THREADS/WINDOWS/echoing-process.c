// echoing-process.c : Defines the entry point for the console application.
//


//#define _CTR_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>   //actual stuff inclided will depend on macros such as _CRT_SECURE_NO_WARNINGS - you can set it manually or via IDE
#include <stdlib.h>

#define ECHOING_EXIT_CODE 2

int main(int argc, char *argv[])
{

	BOOL newprocess;
	STARTUPINFO si;
	PROCESS_INFORMATION  pi;
	DWORD exitcode;
	char buff[128];

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);
	

	if (argc == 2)
	{
		printf("Starting up echoing process ....\n");   
		fflush(stdout);
		newprocess = CreateProcess((LPCSTR)argv[1], (LPSTR)argv[1], NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
		if (newprocess == FALSE)
		{
			printf(" failure\n\n");
			ExitProcess(-1);
		};
		printf(" done\n\n");
		WaitForSingleObject(pi.hProcess, INFINITE);		//il processo parent aspetta la terminazione del processo figlio 
		GetExitCodeProcess(pi.hProcess, &exitcode);		// entrer� qui solo dopo che il prcesso filio avr� terminato
		printf("\nEchoing process exited with code %d\n", exitcode);
		fflush(stdout);

	}
	else
	{
		printf("\tHere I'm for echoing\n\t");
		fflush(stdout);
		while (1)
		{
			scanf("%s", buff);
			if (strcmp(buff, "bye") == 0) ExitProcess(ECHOING_EXIT_CODE);
			printf("\t%s\n\t", buff);
			fflush(stdout);
		}
	}
}

