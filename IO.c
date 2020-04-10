#include "IO.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_CHARS_PORT_STRING 6
#define EXIT_ERROR 1

// taken from: https://stackoverflow.com/a/35202027/9732548
char *myItoa(int num, char *str)
{
  if (str == NULL) {
    return NULL;
  }
  sprintf(str, "%d", num);
  return str;
}

void writePortToFile(char *pathOfFile, int port)
{
  FILE *filePtr = NULL;
  char portString[MAX_CHARS_PORT_STRING] = "";
  filePtr = fopen(pathOfFile, "w");

  if (filePtr == NULL) {
    errorHandle("An error occurred while opening file to write\n");
  }
  myItoa(port, portString);
  fprintf(filePtr, "%s", portString);
  fclose(filePtr);
}

void errorHandle(const char *msg)
{
  perror(msg);
  exit(EXIT_ERROR);
}
