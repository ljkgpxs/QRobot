/* QRobot Project By Ljk
 * 	2016-01-17
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define	TULING_URL	"http://www.tuling123.com/openapi/api"
#define APIKEY_PATH	"./settings.cfg"

void doit(char *text)
{
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json) {
	printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    } else {
	int code = cJSON_GetObjectItem(json, "code")->valueint;
//	printf("Code is %d\n", code);
	char *text = cJSON_GetObjectItem(json, "text")->valuestring;
	printf("@%s\n", text);
    }
}


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
//	printf("Got \n%s\n", ptr);
	doit(ptr);
	return size*nmemb;
}


char *get_apikey()
{
    FILE *fp = fopen(APIKEY_PATH, "r");
    if(fp == NULL) {
	    fprintf(stderr,"Error Get apikey file\n");
    }
    char *apikey = (char *) malloc(100);
    fgets(apikey, 100, fp);
    int i;
    for (i = 0; apikey[i] != '\n'; i++) {
    }
    apikey[i] = '\0';
    return apikey;
}

char *combine_data(char *str1, char *str2, char *str3)
{
    char *post_data = (char *) malloc(2048);
    memset(post_data, 0, 2048);
    strcpy(post_data, "key=");
    strcat(post_data, str1);
    strcat(post_data, "&userid=");
    strcat(post_data, str2);
    strcat(post_data, "&info=");
    strcat(post_data, str3);
    return post_data;
}

int httpget(char *apikey, char *userid, char *text)
{
    CURL *curl;
    CURLcode res;
    char *post_data = combine_data(apikey, userid, text);
// Debug
//	printf("Post Data  %s\n", post_data);
//
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
	curl_easy_setopt(curl, CURLOPT_URL, TULING_URL);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
// Debug
//	printf("Starting... GET\n");
//
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		    fprintf(stderr, "curl_easy_perform() failed: %s\n",
		    curl_easy_strerror(res));
	    curl_global_cleanup();
	    return -1;
	}
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
	fprintf(stderr, "%s [Json file path]", argv[0]);
	return 1;
    }

    httpget(get_apikey(), argv[1], argv[2]);
//    printf("%s\n", combine_data(get_apikey(), argv[2], argv[3]));
    return 0;
}
