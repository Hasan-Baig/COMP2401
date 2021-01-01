#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"


int main() {
  // ... ADD SOME VARIABLES HERE ... //
  int                 clientSocket, addrSize, bytesReceived;
  struct sockaddr_in  clientAddr;
  unsigned char                buffer[4]; 

  // Register with the server
  // ... WRITE SOME CODE HERE ... //
  
  // Create socket
  clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (clientSocket < 0) {
    printf("*** CLIENT ERROR: Could open socket.\n");
    exit(-1);
  }

  // Setup address 
  memset(&clientAddr, 0, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  clientAddr.sin_port = htons((unsigned short) SERVER_PORT);  

  // Send command string to server
  // ... WRITE SOME CODE HERE ... //
  buffer[0] = STOP;

  printf("CLIENT: Sending STOP to server.\n");
  sendto(clientSocket, buffer, sizeof(buffer), 0,
          (struct sockaddr *) &clientAddr, sizeof(clientAddr));

  // Close the socket
  close(clientSocket);  
  printf("CLIENT: Shutting down.\n");

}
