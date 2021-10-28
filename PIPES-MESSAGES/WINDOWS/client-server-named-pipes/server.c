#include <stdio.h>
#include <windows.h>

typedef struct {
	long code;
	char request_string[20];
} request;




int main(int argc, char *argv[])
{

	DWORD tid;
	request r;  //variabile struct
	char  *response = (char*)"done";
	DWORD result;
	HANDLE hThreadHandle;
	HANDLE my_pipe;


	while (1) {

		my_pipe = CreateNamedPipe("\\\\.\\pipe\\serv", 
			PIPE_ACCESS_DUPLEX, 0, PIPE_UNLIMITED_INSTANCES, 
			1024, 1024, 0, NULL);	/*PIPE_UNLIMITED_INSTANCES: posso creare named pipe con lo stesso nome*/


		if (my_pipe == INVALID_HANDLE_VALUE)
		{
			printf("Errore nella chiamata CreateNamedPipe\n");
			fflush(stdout);
			ExitProcess(1);
		}
		if (!ConnectNamedPipe(my_pipe, NULL))  /*attendo che la connesione arrivi, è full duplex la pipe*/
		{
				printf("Errore nella chiamata ConnectNamedPipe\n");
				fflush(stdout);
				ExitProcess(1);
		}

		if (!ReadFile(my_pipe, &r, sizeof(r), &result, NULL))  /*&r = buffer di lettura*/
		{
			printf("Errore nella chiamata readfile\n");
			ExitProcess(1);
		}
	
		printf("service request (code is %d - payload string is = %s)\n", r.code, r.request_string);
		fflush(stdout);

		if (!WriteFile(my_pipe, response, 20, &result, NULL)) //rispondo sulla stessa named pipe con "fatto"
		{
			printf("Errore nella chiamata WriteFile\n");
			ExitProcess(1);
		}

	}

	return(0);
}

