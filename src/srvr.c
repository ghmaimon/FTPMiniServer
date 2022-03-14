#include "../headers/srvr.h"

char *commandStr[] = {
    "dir", "put", "get",
    "del", "quit"
};

int fds, fdc, maxFd, connFd[MAX_CONNECTIONS], numConn, numConnPerClient[MAX_CONNECTIONS];
fd_set readSetTemp, readSet;
int addrClntSize, numBytes;
char *msg;
addr clientAddrs[MAX_CONNECTIONS];
addr addrServr;

boolean addrEq(addr addrs1, addr addrs2) {
    if (addrs1.sin_family != addrs2.sin_family) return FALSE;
    if (addrs1.sin_port = addrs2.sin_port) return FALSE;
    if (addrs1.sin_addr.s_addr != addrs2.sin_addr.s_addr) return FALSE;
    return TRUE;
}

Request getRequest(char *msg) {
    char comm[5];
    char paramettre[SIZE_MSG];
    Request req;
    int i;
    for (i = 0; i < strlen(msg); i++)
        if (msg[i] != ' ') comm[i] = msg[i];
        else {
            i++;
            break;
        }
    memcpy(paramettre, &(msg[i]), SIZE_MSG - i);
    for (i = 0; i < NUM_COMMANDS; i++)
        if (strcmp(comm, commandStr[i]) == 0)
            req.comm = (command) i;
    if (i == NUM_COMMANDS) req.comm = ERROR;
    return req;
}

void addrsInit() {
    addrServr.sin_port = htons(8080);
    addrServr.sin_family = AF_INET;
    memset(&(addrServr.sin_zero), '0', 8);
    inet_aton("127.0.0.1", &(addrServr.sin_addr));
}

void initServer() {
    fds = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fds == -1) {
        printf("Error while creating the server's socket\b");
        exit(EXIT_FAILURE);
    }
    if (bind(fds, (struct sockaddr*)&addrServr, sizeof(struct sockaddr)) == -1) {
        printf("Error while binding the server's socket to the IP address\n");
        exit(EXIT_FAILURE);
    }
    if (listen(fds, MAX_CONNECTIONS) == -1) {
        printff("Error while moving to the listening mode\n");
        exit(EXIT_FAILURE);
    }
    numConn = 0;
    numClnts = 0;
    maxFd = fds;
    FD_SET(fds, &readSet);
}

void receiveData() {
    FD_ZERO(&readSetTemp);
    readSetTemp = readSet;
    select(maxFd + 1, &readSetTemp, NULL, NULL, NULL);
    if (FD_ISSET(fds, &readSetTemp))
        addClient();
    else
        for (int k = 0; k < MAX_CONNECTIONS; k++)
            if (FD_ISSET(connFd[k], &readSetTemp)) {
                fdc = connFd[k];
                numBytes = recv(fdc, msg, SIZE_MSG, 0);
                if (numBytes == 0) deleteClient();
                else if (numBytes == -1) {
                    printf("Error while receiving data\n");
                    exit(EXIT_FAILURE);
                }
                else receiveCommand();
            }
}

void addClient() {
    fdc = accept(fds, (struct sockaddr*)&addrClient, &addrClntSize);
    for (int i = 0; i < numClnts; i++)
        if (addrEq(clientAddrs[i], addrClient))
            if (numConnPerClient[i] == MAX_CONNECTIONS_PER_CLIENT) {
                printf("max connection per client reached, connection rejected!\n");
                close(fdc);
                return;
            } else {
                connFd[numConn] = fdc;
                numConn++;
                if (fdc > maxFd) maxFd = fdc;
                FD_SET(fdc, &readSet);
                numConnPerClient[i]++;
                return;
            }
    connFd[numConn] = fdc;
    numConn++;
    if (fdc > maxFd) maxFd = fdc;
    FD_SET(fdc, &readSet);
    numConnPerClient[numClnts] = 1;
    clientAddrs[numClnts] = addrClient;
    numClnts++;
    return;
}

int main(int argc, char **argv){

    return 0;
}