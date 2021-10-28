// ASCII-vs-UNICODE.c : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

// usando la macro TEXT la stringa può essere in UNICODE o in ASCII, la scelta sarà in base alle impostazioni di compilazione
TCHAR *p = (TCHAR*)TEXT("francesco");  
TCHAR *format = (TCHAR*)TEXT("%ls\n");


int main(int argc, _TCHAR* argv[])
{
	int size;
	size = _tcslen(p);
	printf("size is %d\n", size);
	printf("size of tchar is %d\n", sizeof(TCHAR));


	wprintf(format, p);			//richiede esplicitamente che la stringa sia rappresentata in UNICODE, se ho compilato senza _UNICODE avrò errore 
	wprintf(L"%d\n", 33);		// premetto L per rappresentare una stringa in UNICODE
	wprintf(L"ciao a tutti\n");

	return 0;
}


