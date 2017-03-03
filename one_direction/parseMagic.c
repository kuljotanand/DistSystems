int getFileLen(Request *request, FILE *fp) {
	int len;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp) + 1;
	rewind(fp);

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
	char temp[50];
	time_t cur = time(0);
	struct tm gm = *gmtime(&cur);
	strftime(temp, sizeof(temp), "%a, %d %b %Y %H:%M:%S %Z", &gm);
	return temp;
}

char *getLastMod(Request *req, char *resp) {
	char temp[50];
	struct stat attr;

	time_t cur = time(0);
	struct tm gm = *gmtime(&cur);

	stat(req->http_uri, &attr);
	gmtime_r(&attr.st_mtime, &gm);
	strftime(temp, sizeof(temp), "%a, %d %b %Y %H:%M:%S %Z\n", &gm);
	return temp;
}

char *tisHead(Request *req, char *resp) {
	int isError = 0;
	char len[100];
	char *fname = (char *) malloc(100); // since we don't know the length of val
	FILE *file;

	// broad block, if at any point ../ then invalid
	if (strstr(req->http_uri, "../")) {
		isError = 1;
	}

	fname = req->http_uri ? req->http_uri : "index.html";

	if (strcmp(fname, "/") == 0) {
		strcpy(fname, "index.html");
	}

	file = fopen(fname, "r");

	// error checking
	//  411, 'Length Required' for post
	//  500, 'Not Found'
	//  505, 'HTTP Version not supported'

	if (file) {
		if (len < 0 || isError) {
			strcpy(resp, "HTTP/1.1 404 Not Found");
			// resp = malloc(strlen(bob));
		} else if (!isError) {
			strcpy(resp, "HTTP/1.1 200 OK");
		}

		// give me len as a string
		sprintf(len, "%d", getFileLen(req, file));

		strcat(resp, "\nServer: lisod/1.0");
		strcat(resp, "\nConnection: keep-alive");
		strcat(resp, "\nContent-length: ");
		strcat(resp, len);
		strcat(resp, "\nContent-Type: ");
		strcat(resp, getFileType(req));
		strcat(resp, "\nDate: ");
		strcat(resp, getDate());
		strcat(resp, "\nLast-Modified: ");
		strcat(resp, getLastMod(req, resp));
		strcat(resp, "\r\n");
		// printf("%s\n", resp); strcat(resp, "\n"); return resp;
	}
	// free(fname);
	return resp;
}

char *tisGet(Request *req, char * resp) {
	return "";
}

char *tisPost(Request *req, char * resp) {
	return "";
}

char *parseMagic(char *buf, Request *req, char *sender) {
	// we need to pass by ref
	char resp[1000];

    // look at the type of request HEAD, GET, POST
    if (strcmp(req->http_method, "HEAD") == 0) {
		printf("responding to : %s\n", req->http_method);
		strcpy(sender, tisHead(req, resp));
	} else if (strcmp(req->http_method, "GET") == 0) {
		printf("responding to : %s\n", req->http_method);
		strcpy(sender, tisGet(req, resp));
	} else if (strcmp(req->http_method, "POST") == 0) {
		printf("responding to : %s\n", req->http_method);
		strcpy(sender, tisPost(req, resp));
	} else {
		printf("WTF did you send me?\n");
		// not valid return 405: Method not allowed
	}
	return sender;
}
