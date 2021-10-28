// channel-redirection.c : Defines the entry point for the console application.
//

/******* write stdin data to a file by writing to stdout ********/
//please compile with ASCII settings

//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 1024


int main(int argc, char **argv[])
{
	HANDLE hFile, hStdout, hStdin;
	DWORD length, filePointer, dummy;
	char input_buffer[BUFFER_SIZE];
	DWORD res;


	hFile = CreateFileA((LPCSTR)argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Cannot open target file\n");
		return -1;
	}

	printf("will redirect stdout-handle to file named: %s - give me the input\n", argv[1]);
	fflush(stdout);
	
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetStdHandle(STD_OUTPUT_HANDLE, hFile);  //per la entry associata allo stadard output voglio che venga duplicato il canale creato dall'apertura del file A
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	while (1)
	{
		ReadFile(hStdin, (LPVOID)input_buffer, BUFFER_SIZE, &res, NULL);  //leggo da standard input
		WriteFile(hStdout, (LPVOID)input_buffer, res, &dummy, NULL);  //scrivo su standard output ma è stato ridirezionato quindi mi porta a scrivere sul file
	}

	return 0;
}




