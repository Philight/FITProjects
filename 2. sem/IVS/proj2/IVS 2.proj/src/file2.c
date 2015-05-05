#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <proj2.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int file_save(char *url, char filename[])
{
	CURL *curl;
	FILE *fp;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) 
	{
		fp = fopen(filename,"wb");
		if(fp == NULL) 
		{
			fprintf(stderr,"Chyba pri otvarani subora\n");
			return(-1);
		}

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		
		curl_easy_cleanup(curl); // clean
		fclose(fp);
	}
}


