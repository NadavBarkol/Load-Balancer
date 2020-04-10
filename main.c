#include "IO.h"
#include "httpConnection.h"
#include "socketConnection.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_SERVERS 3
#define SERVER_PORT_FILE_PATH "server_port"
#define HTTP_PORT_FILE_PATH "http_port"
#define HTTP_REQUEST 1
#define HTTP_ANSWER 2

void receiveHttpAndExecute(int *httpSocket, int *httpListenSocket, int *serverSockets)
{
  int serversIndex = 0;
  char *messageBuffer;
  int sizeOfMessage = 0;

  while (!acceptConnection(httpSocket, httpListenSocket)) {
    messageBuffer = malloc(MESSAGE_BLOCK_SIZE);
    sizeOfMessage = receiveCompleteMessage(messageBuffer, *httpSocket, HTTP_REQUEST);
    sendCompleteMessage(messageBuffer, serverSockets[serversIndex], sizeOfMessage);
    free(messageBuffer);
    messageBuffer = malloc(MESSAGE_BLOCK_SIZE);
    sizeOfMessage = receiveCompleteMessage(messageBuffer, serverSockets[serversIndex], HTTP_ANSWER);
    sendCompleteMessage(messageBuffer, *httpSocket, sizeOfMessage);
    free(messageBuffer);

    serversIndex++;
    serversIndex = serversIndex % NUM_OF_SERVERS;
    close(*httpSocket);
  }
}

int main()
{
  int httpPort, serverPort;
  int i = 0;
  int serverSockets[NUM_OF_SERVERS], serversListenSocket, httpSocket, httpListenSocket;
  srand(time(NULL));
  socketBindAndListen(&serverPort, &serversListenSocket);
  writePortToFile(SERVER_PORT_FILE_PATH, serverPort);
  socketBindAndListen(&httpPort, &httpListenSocket);
  writePortToFile(HTTP_PORT_FILE_PATH, httpPort);

  for (i = 0; i < NUM_OF_SERVERS; i++) {
    acceptConnection(&serverSockets[i], &serversListenSocket);
  }
  receiveHttpAndExecute(&httpSocket, &httpListenSocket, serverSockets);

  closeSocketsList(serverSockets, NUM_OF_SERVERS);
  close(serversListenSocket);
  close(httpListenSocket);
  return 0;
}
