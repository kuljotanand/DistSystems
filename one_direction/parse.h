#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0

//Header field
typedef struct {
	char header_name[4096];
	char header_value[4096];
} Request_header;

//HTTP Request Header
typedef struct {
	char http_version[50];
	char http_method[50];
	char http_uri[4096];
	Request_header *headers;
	int header_count;
} Request;

//HTTP Request Header
// typedef struct {
// 	char *Connection = 'keep-alive';
// 	char *ContentType;
// 	int ContentLength;
// 	char *LastModified;
// 	char *http_version;
// 	char *message_body
// } Response;

Request* parse(char *buffer, int size,int socketFd);
