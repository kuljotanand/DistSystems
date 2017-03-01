char *get_header_value(Request *request, char *name) {
	int i;
	for(i=0; i<request->header_count; i++) {
		if(strcmp(request->headers[i].header_name, name) == 0) {
			return request->headers[i].header_value;
		}
	}

	// if header not set, return special string
	char *nullstr = "NULL";
	return nullstr;
}

void parseMagic() {
    // char *headersize = char *calloc(8192)

    Request *request = parse(buf, sizeof(buf), i);
    // printf ("%s\n", request->http_version);
    // printf ("%s\n", request->http_method);
    // printf ("%s\n", request->http_uri);
    // printf ("%s\n\n", request->headers[0].header_value);

      // request->headers[]

    // strcpy(test, get_header_value(request, "Host"));
    // test = get_header_value(request, "Host");
    // printf ("%s \n", get_header_value(request, "User-Agent"));

    char *avash = "avash";
    strcpy(buf, avash);
    printf("%s\n", avash);

    

    // we got some data from a client
    if (send(i, buf, nbytes, 0) == -1) {
        perror("send");
    }
}

// int codeMatcher() {
// 	switch(value) {
// 	   case '' :
// 	      break;
// 	   case '' :
// 	      break;
// 	   default :
// 	}
// 	return 0;
// }
