#include <stdio.h>
#include <proj2.h>

int main(int argc, char* argv[]) 
{
	if (argc == 3)
	{
		file_save(argv[1],argv[2]);
		parity_check(argv[2]);
		printf("\n");
	}
	else if (argc == 2)
	{
		parity_check(argv[1]);
		printf("\n");
	}
	else
		fprintf(stderr,"Zle argumenty\n");

	return 0;
}
