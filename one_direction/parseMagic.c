int getFileLen(Request *request) {
	int len;
	FILE *fp = fopen(request->http_uri, "r");

	if (!fp)  {
		perror ("Error opening file");
		return(-1);
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp) + 1;
	rewind(fp);
	// fclose(fp);
	return len;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

char *getFileType(Request *req) {
	char *type;
	const char *ext = get_filename_ext(req->http_uri);

	if (strcmp(ext, "html") == 0) {
		type = "text/html";
	} else if (strcmp(ext, "css") == 0) {
		type = "text/css";
	} else if (strcmp(ext, "png") == 0) {
		type = "image/png";
	} else if (strcmp(ext, "jpeg") == 0) {
		type = "image/jpeg";
	} else if (strcmp(ext, "gif") == 0) {
		type = "image/gif";
	} else if (strcmp(ext, "txt") == 0) {
		type = "text/plain";
	} else { // default
		type = "application/octet-stream";
	}
	return type;
}

char *getDate() {
	char *temp;
	time_t now = time(0);
	struct tm gm = *gmtime(&now);
	strftime(temp, sizeof(temp), "%a, %d %b %Y %H:%M:%S %Z", &gm);
	return temp;
}

char *getLastMod(Request *req, char *resp) {
	char *temp;
	struct stat attr;

	time_t now = time(0);
	struct tm gm = *gmtime(&now);

	stat(req->http_uri, &attr);
	gmtime_r(&attr.st_mtime, &gm);
	strftime(temp, sizeof(temp), "%a, %d %b %Y %H:%M:%S %Z\n", &gm);
	return temp;
}

char *tisHead(Request *req, char *resp) {
	// broad block, if at any point ../ then invalid
	int isError = 0;
	int len;

	if (!strstr(req->http_uri, "../")) {
		isError = 1;
	}

	if (access(req->http_uri, F_OK) != -1) {
		len = getFileLen(req);

		if (len < 0 || isError) {
			strcpy(resp,"HTTP/1.1 500 Internal Server Error");
			return;
		} else if (!isError) {
			strcpy(resp,"HTTP/1.1 200 OK");
		}

		strcat(resp, "\nServer: lisod/1.0");
		strcat(resp, "\nConnection: keep-alive"); // \r\n
		strcat(resp, "\nContent-length: ");
		strcat(resp, len);
		strcat(resp, "\nContent-Type: ");
		strcat(resp, getFileType(req));
		strcat(resp, "\nDate: ");
		strcat(resp, getDate());
		strcat(resp, "\nLast-Modified: ");
		strcat(resp, getLastMod(req, resp));
		strcat(resp, "\r\n");
	}
	return resp;
}

char *tisGet(Request *req, char * resp) {
	return "";
}

char *tisPost(Request *req, char * resp) {
	return "";
}

char *parseMagic(char *buf, Request *req) {
	// we need to pass by ref
	char *resp = "";
	char *send = "";

    // look at the type of request HEAD, GET, POST
    if (strcmp(req->http_method, "HEAD") == 0) {
		printf("responding to : %s\n", req->http_method);
		send = tisHead(req, resp);
	} else if (strcmp(req->http_method, "GET") == 0) {
		printf("responding to : %s\n", req->http_method);
		send = tisGet(req, resp);
	} else if (strcmp(req->http_method, "POST") == 0) {
		send = tisPost(req, resp);
	} else {
		// not valid return 405: Method not allowed
	}
	return send;
	// do something with send, namely send it
		//  404, 'Not Found'
	    //  411, 'Length Required'
	    //  500, 'Not Found'
	    //  505, 'HTTP Version not supported'

    // Request *request = parse(buf, sizeof(buf), i);
    // printf ("%s \n", get_header_value(request, "User-Agent"));
}
