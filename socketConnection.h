#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

void socketBindAndListen(int *portToListen, int *socketToListen);
int acceptConnection(int *connectionSocket, int const *listeningSocket);
void closeSocketsList(int *socketsArr, int numOfSockets);

#endif  // SOCKETCONNECTION_H
