#include "socketConnection.h"
#include "IO.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define LOWER_PORT_NUM 1024
#define UPPER_PORT_NUM 64000
#define MAX_CONNECTIONS_QUEUE 3
#define ERROR_BIND_RESULT -1
#define SUCCESS_ACCEPTING_CONNECTION 0
#define RANDOM_OPTIONS 2
#define OCCUPIED_PORT 80

// reference: https://stackoverflow.com/questions/1202687/how-do-i-get-a-specific-range-of-numbers-from-rand
void randomizePort(int *portToRand)
{
  int randomOrOccupied;
  *portToRand = rand() % (UPPER_PORT_NUM + 1 - LOWER_PORT_NUM) + LOWER_PORT_NUM;
  randomOrOccupied = rand() % RANDOM_OPTIONS;
  if (randomOrOccupied == 0) {
    *portToRand = OCCUPIED_PORT;
  }
}

void socketBindAndListen(int *portToListen, int *socketToListen)
{
  int bindResult = ERROR_BIND_RESULT;
  struct sockaddr_in servAddr;

  *socketToListen = socket(AF_INET, SOCK_STREAM, 0);
  if (*socketToListen < 0) {
    errorHandle("ERROR opening socket");
  }
  while (bindResult == ERROR_BIND_RESULT) {
    randomizePort(portToListen);

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(*portToListen);

    bindResult = bind(*socketToListen, (struct sockaddr *)&servAddr, sizeof(servAddr));
  }
  listen(*socketToListen, MAX_CONNECTIONS_QUEUE);
}

int acceptConnection(int *connectionSocket, int const *listeningSocket)
{
  socklen_t cliLen;
  struct sockaddr_in cliAddr;
  cliLen = sizeof(cliAddr);
  *connectionSocket = accept(*listeningSocket, (struct sockaddr *)&cliAddr, &cliLen);
  if (*connectionSocket < 0) {
    errorHandle("ERROR on accept");
  }
  return SUCCESS_ACCEPTING_CONNECTION;
}

void closeSocketsList(int *socketsArr, int numOfSockets)
{
  int i = 0;
  for (i = 0; i < numOfSockets; i++) {
    close(socketsArr[i]);
  }
}
