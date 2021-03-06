// copy.c : Defines the entry point for the console application.
// please compile with ASCII settings
//

//#include "stdafx.h"

#include <windows.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {   

	HANDLE sd, dd; 
	DWORD size, result; 
	char buffer[BUFSIZE];	 
	
	if (argc != 3) {	/* Controllo sul numero di argomenti */
		printf("usage: copia source target\n");  
		return -1;
	}   
	
	/* Apertura del file da copiare in sola lettura */
	sd=CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (sd == INVALID_HANDLE_VALUE) {
		printf("Cannot open source file\n");
		return -1;
	}

	/* Creazione del file destinazione */
	dd = CreateFile(argv[2], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (dd == INVALID_HANDLE_VALUE) {
		printf("Cannot open destination file\n");
		return -1;
	}

	do { /* Qui iniziano le operazioni di copia */
		if (ReadFile(sd, buffer, BUFSIZE, &size, NULL) == 0) { 
			printf("Cannot read from source file\n");        
			return -1; 
		}
		if (WriteFile(dd, buffer, size, &result, NULL) == 0){ 
			printf("Cannot write to destination file\n");           
			return -1; 
		} 
	} while (size > 0);
	
	CloseHandle(sd);
	CloseHandle(dd);
	
	return 0;
}

