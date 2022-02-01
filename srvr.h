#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_CONNECTIONS 50
#define MAX_CONNECTIONS_PER_CLIENT 5
#define TAILLE_MOT 256
#define NUM_COMMANDS 5

typedef enum {
    TRUE, FALSE
} boolean;

typedef enum {
    DIR, PUT, GET,
    DEL, QUIT, ERROR
} command;

extern char *commandStr[];

typedef struct {
    command comm;
    char parametre[TAILLE_MOT];
} Request;

typedef struct sockaddr_in addr;

extern int fds, fdc, maxFd, connFd[MAX_CONNECTIONS], numConn, numConnPerClient[MAX_CONNECTIONS];
extern fd_set readSetTemp, readSet;
extern int addrClntSize;
extern char *msg;
extern addr clientAddrs[MAX_CONNECTIONS];
extern addr addrServr;

boolean addrEq(addr, addr);
Request getRequest(char *);
void addrsInit(addr *);
void initServer();
void receiveData();
void addClient();
void deleteClient();
void receiveCommand();