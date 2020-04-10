#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#define MESSAGE_BLOCK_SIZE 512

int receiveCompleteMessage(char *messageBuffer, int currentSocket, int httpMessageType);
void sendCompleteMessage(char *messageBuffer, int currentSocket, int sizeOfMessage);
#endif  // HTTPCONNECTION_H
