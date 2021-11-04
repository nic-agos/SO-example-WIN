
// ACL-access.c : Defines the entry point for the console application.
// you can compile with either ASCII or UNICODE
//

#
//#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <Sddl.h>
#include "accctrl.h"
#include "aclapi.h"
#pragma comment(lib, "advapi32.lib")

int main(void)
{
	DWORD dwRtnCode = 0;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;
	TCHAR *stringSID;


	// Get the handle of the file object.
	hFile = CreateFile(
		TEXT("myfile.txt"),  //TEXT macro universale quando non sappiamo a priori se compilo con ASCII o UNICODE
		GENERIC_READ,
		FILE_SHARE_READ,  //altre sessioni possono leggere il file
		NULL,  //ACL di default
		CREATE_ALWAYS, //|OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Check GetLastError for CreateFile error code.
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError(); //permette di capire il vero codice numerico dell'errore
		_tprintf(TEXT("CreateFile error = %d\n"), dwErrorCode);  //_tprintf perchè non sappiamo se ASCII o UNICODE
		return -1;
	}

	// Get the owner SID of the file.
	dwRtnCode = GetSecurityInfo(
		hFile,  //handle alla sessione sul file
		SE_FILE_OBJECT,
		OWNER_SECURITY_INFORMATION,  //voglio anche l'owner tra le informazioni che ci vengono date
		&pSidOwner,  //luogo dover verrà scritto il SID dell'owner
		NULL,
		NULL,
		NULL,
		&pSD);  //area di memoria dove verrà scritta l'ACL

	printf("this is the owner address: %p\nthis is the structure address: %p\n", pSidOwner, pSD);
	// Check GetLastError for GetSecurityInfo error condition.
	if (dwRtnCode != ERROR_SUCCESS)
	{
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		_tprintf(TEXT("GetSecurityInfo error = %d\n"), dwErrorCode);
		return -1;
	}

	ConvertSidToStringSid(pSidOwner, &stringSID);  //converto l'owner da maschera di bit a stringa legibile 

	_tprintf(TEXT("this is the owner SID: %s\n"), stringSID);  //stampa in formato stringa il SID dell'owner


	AcctName = (LPTSTR)malloc(4096);
	DomainName = (LPTSTR)malloc(4096);

	dwAcctName = 4096;
	dwDomainName = 4096;

	// una volta che conoscoo il SID posso ottenere più informazioni sull'account
	bRtnBool = LookupAccountSid(
		NULL,                   // name of local or remote computer
		pSidOwner,              // security identifier
		AcctName,               // account name buffer
		(LPDWORD)&dwAcctName,   // size of account name buffer 
		DomainName,             // domain name
		(LPDWORD)&dwDomainName, // size of domain name buffer
		&eUse);                 // SID type

	// Print the account name.
	_tprintf(TEXT("Account owner = %s\n"), AcctName);

	return 0;
}


