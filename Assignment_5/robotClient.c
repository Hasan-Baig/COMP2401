#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"


// This is the main function that simulates the "life" of the robot
// The code will exit whenever the robot fails to communicate with the server
int main() {

  int                 clientSocket, addrSize, bytesReceived;
  struct sockaddr_in  clientAddr;
  unsigned char       buffer[80];   // stores sent and received data
  
  //values stored in buffer
  unsigned short      robotID;
  unsigned short 		  xHigh;
  unsigned short	    xLow;
	unsigned short      yHigh;
  unsigned short 	    yLow;
  unsigned short     	randDirection = 0;
	unsigned short      randMagnitude = 0;

  float               robotX;
  float               robotY;
  int                 robotDirection;

  int                 turnDirection;
  int                 keepSameDirection = 0; //flag to keep turning same direction 

  

  // Set up the random seed
  srand(time(NULL));
  
  // ******************************************************** REGISTER WITH SERVER ********************************************************

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

  // ******************************************************** REGISTER COMMAND ********************************************************

  // Send register command to server.  Get back response data
  // and store it.   If denied registration, then quit.
  buffer[0] = REGISTER;

  //Send REGISTER to server
  sendto(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
  
  // Get response from server
  addrSize = sizeof(clientAddr);
  bytesReceived = recvfrom(clientSocket, buffer, 80, 0, (struct sockaddr *) &clientAddr, &addrSize);
  buffer[bytesReceived] = 0; // put a 0 at the end so we can display the string
  
  if (buffer[0] == OK) {
    robotID = (unsigned short)(buffer[1]);
    xHigh =   (unsigned short)(buffer[2]);
    xLow =    (unsigned short)(buffer[3]);
    yHigh =   (unsigned short)(buffer[4]);
    yLow =    (unsigned short)(buffer[5]);
    randDirection = (unsigned short)(buffer[6]);
    randMagnitude = (unsigned short)(buffer[7]);

    robotX = (float)((xHigh*256) + xLow);
    robotY = (float)((yHigh*256) + yLow);

    if (randDirection == 0){
    	robotDirection = (int)randMagnitude;
    } else if (randDirection == 1) {
    	robotDirection = -(int)randMagnitude;
    }

  } else if (buffer[0] == NOT_OK) {
    printf("CLIENT: MAX_ROBOTS capacity reached, quitting.\n");
    close(clientSocket);
    printf("CLIENT: Shutting down.\n");
    exit(-1);
  }

  // ******************************************************** CHECK_COLLISION COMMAND ********************************************************
  
  // Go into an infinite loop exhibiting the robot behavior
  while (1) {
    // Check if can move forward
    buffer[0] = CHECK_COLLISION;
    robotID = (unsigned short)(buffer[1]);
    xHigh =   (unsigned short)(buffer[2]);
    xLow =    (unsigned short)(buffer[3]);
    yHigh =   (unsigned short)(buffer[4]);
    yLow =    (unsigned short)(buffer[5]);
    randDirection = (unsigned short)(buffer[6]);
    randMagnitude = (unsigned short)(buffer[7]);

    robotX = (float)((xHigh*256) + xLow);
    robotY = (float)((yHigh*256) + yLow);

    if (randDirection == 0){
    	robotDirection = (int)randMagnitude;
    } else if (randDirection == 1) {
    	robotDirection = -(int)randMagnitude;
    }

    //Send CHECK_COLLISION to server 
    sendto(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    
    // Get response from server
    addrSize = sizeof(clientAddr);
    bytesReceived = recvfrom(clientSocket, buffer, 80, 0, (struct sockaddr *) &clientAddr, &addrSize);
    buffer[bytesReceived] = 0; // put a 0 at the end so we can display the string

    if (buffer[0] == OK) {
      // If ok, move forward     
      robotX = robotX+(ROBOT_SPEED*cos(robotDirection/180*PI));
      robotY = robotY+(ROBOT_SPEED*sin(robotDirection/180*PI));

      //store high/low bytes for new location
      xHigh = (unsigned short)robotX / 256;
      xLow =  (unsigned short)robotX % 256;
      yHigh = (unsigned short)robotY / 256;
      yLow =  (unsigned short)robotY % 256;

      //response
      buffer[2] = (unsigned char)xHigh;
      buffer[3] = (unsigned char)xLow;
      buffer[4] = (unsigned char)yHigh;
      buffer[5] = (unsigned char)yLow;

      keepSameDirection = 0; //choose new direction

    } else if ((buffer[0] == NOT_OK_BOUNDARY) || (buffer[0] == NOT_OK_COLLIDE)) {
      // Otherwise, we could not move forward, so make a turn.
      // If we were turning from the last time we collided, keep
      // turning in the same direction, otherwise choose a random 
      
      // direction to start turning.
      if (keepSameDirection == 0){ 
        turnDirection = (int)(rand() % 2); //0 for left (CCW) & 1 for right (CW)
      }

      //turning
      if (turnDirection == 0){
        // newRobotDirection = robotDirection+ROBOT_TURN_ANGLE;
        robotDirection = robotDirection+ROBOT_TURN_ANGLE;
      } else if (turnDirection == 1) {
        // newRobotDirection = robotDirection-ROBOT_TURN_ANGLE;
        robotDirection = robotDirection-ROBOT_TURN_ANGLE;
      }
      
      //keep the same turn direction
      keepSameDirection = 1;
      
      //response
      if(robotDirection < 0){
        buffer[6] = (unsigned char)1; //direction (negative)
        buffer[7] = (unsigned char)(-robotDirection); //magnitude
      } else {
        buffer[6] = (unsigned char)0; //direction (positive)
        buffer[7] = (unsigned char)robotDirection; //magnitude
      }

    } else if (buffer[0] == LOST_CONTACT) {
      close(clientSocket); 
      printf("CLIENT: Shutting down.\n");
    }       

    // ******************************************************** STATUS_UPDATE COMMAND ********************************************************

    // Send STATUS_UPDATE to server
    buffer[0] = STATUS_UPDATE;
    sendto(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));

    // Uncomment line below to slow things down a bit 
    usleep(20000);
  }
}

