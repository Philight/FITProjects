#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <proj2.h>

int parity_check(char filename[])
{
	FILE *fp;
	int c;

	int evenint;	

	fp = fopen(filename,"r");
	if(fp == NULL) 
		{
			fprintf(stderr,"Chyba pri otvarani subora\n");
			return(-1);
		}

	do
	{
		c = fgetc(fp);
		if( feof(fp) )
		{
			break ;
		}

		evenint += isOddParity(c); // 

	}while(1);
	
	
	evenint % 2 ? printf("0") : printf("1"); // parna/suda = 1, neparna/licha = 0
	
	fclose(fp);
	return 0;
}


/* 
 * Return 1 if odd, 0 if even.
 */

int isOddParity(unsigned char filechar)
{
	int parity = 0;

	for (filechar &= ~0x80;  filechar != 0;  filechar >>= 1)
		parity ^= (filechar & 1);

	return parity;
}


