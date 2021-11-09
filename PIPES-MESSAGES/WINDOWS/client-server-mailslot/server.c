/*********************************************************/
/*************** server process **************************/
/*********************************************************/

#include <windows.h>
#include <stdio.h>
#include <malloc.h>

#define SIZE  128
#define MAX_NAME_SIZE  128
#define MULTITHREAD

typedef struct {
	char queue_name[MAX_NAME_SIZE];  //nome del mailSlot al quale il server invierà la risposta per destinarla al client
	int service_code; //ospiterà il codice della richiesta che proviene dal client
} request;


typedef struct {
	request req;
} request_msg;


typedef struct {
	char mtext[SIZE];
} response_msg;


HANDLE my_slot;
char *response = (char*)"done";

void * service_thread(request_msg *request_message)
{
	response_msg response_message;
	HANDLE response_slot;
	DWORD writtenchars;

	printf("asked service of type %d - response channel is %s\n", request_message->req.service_code, request_message->req.queue_name);


	/* do the service */

	//apro in scrittura il mailslot creato dal client
	response_slot = CreateFile(  
		request_message->req.queue_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,  // required to write to a mailslot
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (response_slot == INVALID_HANDLE_VALUE)
	{
		printf("cannot open response queue to the client\n");
		ExitThread(1);
	}

	memcpy(response_message.mtext, response, 10); //scrivo 10 byte sulla struct usata per rispondere, il contenuto sarà la stringa contenuta in response
	if (WriteFile(response_slot, &response_message,	SIZE, &writtenchars, NULL) == 0)  //scrivo la risposta sul mailslot del client
	{
		printf("cannot return response to the client\n");
		ExitThread(-1);
	}
	free(request_message);  //dealloco il buffer creato con malloc nel main, se siamo multithread potrebbero venire sovrascritte richieste diverse
}

int main(int argc, char *argv[])
{

	void * request_message;
	DWORD thread_id;
	DWORD readchar;

	my_slot = CreateMailslot("\\\\.\\mailslot\\server",
		sizeof(request_msg),
		MAILSLOT_WAIT_FOREVER,
		NULL);


	if (my_slot == INVALID_HANDLE_VALUE)
	{
		printf("cannot install server queue, please check the problem\n");
		exit(1);
	}


	while (1)
	{
		request_message = malloc(sizeof(request_msg));  
		// Cerco di ottenere la richiesta proveniente dal client e la salvo nella variabile locale request_message
		if (ReadFile(my_slot, request_message, sizeof(request_msg), &readchar, NULL) == 0)  //in &readchar byte effettivamente consegnati
		{
			printf("message receive error, please check the problem\n");
			ExitProcess(1);
		}
		else
		{

#ifdef MULTITHREAD
			//creo un thread che si fa carico lui di rispondere alla richiesta del client
			if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)service_thread, request_message, 0, &thread_id)
				== INVALID_HANDLE_VALUE)
			{
				printf("Cannot create service thread for error %d\n", GetLastError());
				ExitProcess(-1);
			}

#endif

#ifndef MULTITHREAD
			service_thread((request_msg*)request_message);
#endif

		}
	}/* end while */
}/* end main*/

