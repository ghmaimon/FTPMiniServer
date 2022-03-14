#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// server's properties
#define MAX_CONNECTIONS 50
#define MAX_CONNECTIONS_PER_CLIENT 5
#define SIZE_MSG 256
#define NUM_COMMANDS 5


// definition of type boolean
typedef enum {
    FALSE, TRUE
} boolean;

// possible commands
typedef enum {
    DIR, PUT, GET,
    DEL, QUIT, ERROR
} command;

// a string to hold the current passed command
extern char *commandStr[];

// definition of the request structure
typedef struct {
    command comm;
    char parametre[SIZE_MSG];
} Request;

// internet ipv4 address
typedef struct sockaddr_in addr;

// global variables
extern int fds, fdc, maxFd, connFd[MAX_CONNECTIONS], numConn, numClnts, numConnPerClient[MAX_CONNECTIONS];
extern fd_set readSetTemp, readSet;
extern int addrClntSize;
extern char *msg;
extern addr clientAddrs[MAX_CONNECTIONS];
extern addr addrServr, addrClient;


// prototypes of functions :

// the following function compares two ipv4 addresses and returns TRUE if they were equal
boolean addrEq(addr, addr);

// the following function extracts a formal request (with command and parametre) from a given message.
Request getRequest(char *);

// this function initializes the server's ipv4 address.
void addrsInit();

// this function initializes the server
void initServer();

// this function receives any data to the server, it might be a request or a connection/deconnection of a client
void receiveData();

// this function adds a client to the connected clients.
void addClient();

// this function removes a client from the connected clients.
void deleteClient();

// this function receive a command from a client.
void receiveCommand();