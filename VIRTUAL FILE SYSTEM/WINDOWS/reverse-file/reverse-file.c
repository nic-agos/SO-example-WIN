/******* write strings to a file in reverse order ********/
//please compile with ASCII settings

//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 1024

#define OPTIMIZED_BUFFER_MANAGEMENT 


void Error(char * message) {
	puts(message);
	ExitProcess(-1);
}


int main(int argc, char **argv[])
{
	HANDLE hReverseFile;
	DWORD length, filePointer, dummy;
	char buffer[2][BUFFER_SIZE]; //array di 2 elementi, ciascuno di questi è un buffer (matrice)
	char *input_buffer, *temp_buffer; 
	DWORD res;
	int i = 0;

	printf("file name is: %s - give me the input\n", argv[1]);
	/*CREATE_ALWAYS crea un nuovo file se non esiste, se esiste già e si hanno i permessi per scriverci lo sovrascrive*/
	hReverseFile = CreateFileA((LPCSTR)argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hReverseFile == INVALID_HANDLE_VALUE)
	{
		Error("Cannot open reverse file\n");
	}

	input_buffer = buffer[i];  //buffer di lettura
	temp_buffer = buffer[i + 1];  //buffer dove salvo temporaneamente i dati da file che verrebbero sovrascrittti dalla nuova scrittura

	while (1)
	{
		scanf("%s", input_buffer);
		//printf("%s\n",input_buffer);
		//fflush(stdout);
		if (strcmp(input_buffer, ".") == 0)
		{
			break;
		}

		length = strlen(input_buffer);
		input_buffer[length++] = '\n'; //scrivo il file organizzandolo per righe, quindi metto a capo alla fine di ogni input letto
		SetFilePointer(hReverseFile, 0, NULL, FILE_BEGIN);  //mi posiziono all'inizio del file
		do
		{
			ReadFile(hReverseFile, (LPVOID)temp_buffer, length, &res, NULL);  //salvo sul buffer temporaneo la parte che già presente nel file verrebbe sovrascritta perchè all'inizio del file
			filePointer = SetFilePointer(hReverseFile, -res, NULL, FILE_CURRENT);  //per posizionarmi indietro all'inizio della zona già letta che è quella da sovrascrivere
			//printf("file pointer is %d",filePointer);
			//fflush(stdout);
			WriteFile(hReverseFile, (LPVOID)input_buffer, length, &dummy, NULL);

#ifdef OPTIMIZED_BUFFER_MANAGEMENT
			i = (i++);
			//inverto i pointer tra i buffer per evitare la memcpy
			input_buffer = buffer[i % 2];  
			temp_buffer = buffer[(i + 1) % 2];
#else
			memcpy(input_buffer, temp_buffer, res);  //scrivo sul buffer di input i dati precedentementa salvati sul buffer temporaneo (quelli che sarebbero stati sovrascritti) per re-inserirli nel file
#endif
			length = res;
		} while (res>0);

	}

	return 0;
}



