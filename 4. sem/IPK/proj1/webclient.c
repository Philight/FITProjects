
#include <stdio.h>

/*
 * Enum pre rozne vstupy
 */
typedef enum
{
	HELP = 1,
	TEST,
	RPATH,
	LPATH
} EArg;

/*
 * Enum pre errory/chyby
 */
typedef enum
{
	E_OK,
	E_PARAMETER,
	E_SOCKET_CREATE,
	E_NEDOSTATOK_PAMATE

} EError;

EError print_error(EError errorCode)
{
	switch(errorCode)
	{
		case (E_SOCKET_CREATE):
			printf("Error opening the socket\n");
			break;
		case (E_SOCKET_CLOSE):
			printf("Error closing the socket\n");
			break;
	}
}

int main(int argc, char *argv[])
{
	int sock;
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        print_error(E_SOCKET_CREATE);

    if(close(sock) < 0) 
        printCodeExit(E_SOCKET_CLOSE);
    
	return 0;
}