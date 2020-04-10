#include "httpConnection.h"
#include "IO.h"
#include <stdlib.h>
#include <sys/socket.h>

#define END_STRING_LENGTH 4
#define NOT_END_OF_MESSAGE -1
#define TRANSFER_ERROR -1
#define FIRST_N_OFFSET 1
#define SECOND_R_OFFSET 2
#define SECOND_N_OFFSET 3
#define OFFSET_BEFORE_LAST_FLAG 1
#define LAST_CHAR_LEN 1

int isEndOfMessage(int messageTotalNumOfBytes, char const *messageBuffer, int httpMessageType)
{
  int i = 0;
  int messageLength = 0;
  int numOfFlagAppearances = 0;

  for (i = 0; i < messageTotalNumOfBytes - END_STRING_LENGTH + LAST_CHAR_LEN; i++) {
    if (messageBuffer[i] == '\r' && messageBuffer[i + FIRST_N_OFFSET] == '\n' &&
        messageBuffer[i + SECOND_R_OFFSET] == '\r' && messageBuffer[i + SECOND_N_OFFSET] == '\n') {
      if (numOfFlagAppearances == httpMessageType - OFFSET_BEFORE_LAST_FLAG) {
        messageLength = i + END_STRING_LENGTH;
        return messageLength;
      } else {
        numOfFlagAppearances++;
      }
    }
  }
  return NOT_END_OF_MESSAGE;
}

int receiveCompleteMessage(char *messageBuffer, int currentSocket, int httpMessageType)
{
  int endOfMessageFlag = NOT_END_OF_MESSAGE;
  char *currentMsgBlockPtr = messageBuffer;
  int numOfBytesReceivedInBlock = 0;
  char *reallocResult = 0;
  int messageTotalNumOfBytes = 0;
  while (endOfMessageFlag < 0) {
    numOfBytesReceivedInBlock = recv(currentSocket, currentMsgBlockPtr, MESSAGE_BLOCK_SIZE, 0);
    if (numOfBytesReceivedInBlock == TRANSFER_ERROR) {
      errorHandle("ERROR on receiving message");
    }
    messageTotalNumOfBytes = messageTotalNumOfBytes + numOfBytesReceivedInBlock;
    endOfMessageFlag = isEndOfMessage(messageTotalNumOfBytes, messageBuffer, httpMessageType);
    currentMsgBlockPtr += numOfBytesReceivedInBlock;
    if (endOfMessageFlag < 0) {
      reallocResult = realloc(messageBuffer, MESSAGE_BLOCK_SIZE);
      if (reallocResult == NULL) {
        errorHandle("realloc failed\n");
      }
    }
  }
  return endOfMessageFlag;
}

void sendCompleteMessage(char *messageBuffer, int currentSocket, int sizeOfMessage)
{
  char *CurrentPlaceAtBuffer = messageBuffer;
  int BytesTransferred = 0;
  int RemainingBytesToSend = sizeOfMessage;

  while (RemainingBytesToSend > 0) {
    BytesTransferred = send(currentSocket, CurrentPlaceAtBuffer, RemainingBytesToSend, 0);
    if (BytesTransferred == TRANSFER_ERROR) {
      errorHandle("ERROR on sending message");
    }
    RemainingBytesToSend = RemainingBytesToSend - BytesTransferred;
    CurrentPlaceAtBuffer = CurrentPlaceAtBuffer + BytesTransferred;
  }
}
