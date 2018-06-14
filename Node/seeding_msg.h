#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* getaddrinfo() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define REGISTER_IP 1
#define REQUEST_IP 2
#define REGISTER_BTCADDR 3
#define REQUEST_BTCADDR 4


typedef struct{
    int op;
    char message[256];
} MSG;
