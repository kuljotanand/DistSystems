#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9037"   // port we're listening on

fd_set master;    // master file descriptor list
fd_set read_fds;  // temp file descriptor list for select()
int fdmax;        // maximum file descriptor number

int listener;     // listening socket descriptor
int newfd;        // newly accept()ed socket descriptor
struct sockaddr_storage remoteaddr; // client address
socklen_t addrlen;

char buf[8192];    // buffer for client data
int nbytes;

char remoteIP[INET6_ADDRSTRLEN];

int yes = 1;        // for setsockopt() SO_REUSEADDR, below
int i, rv;

struct addrinfo hints, *ai, *p;
