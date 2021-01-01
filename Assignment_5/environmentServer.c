/*
* NOTE FOR TA:
*  
* Everything seems to work fine, however when the new values of location/direction to move forward are calculated 
* in client only the x-value is changed and the y-value stays the same. (In server the same calculation is done
* properly with different x & y values, I dont know why there is a difference).
* 
* Therefore robot always moves to the right (also confused why only to the right and not left). When adding a new
* robot the new ID is set for both robots causing them to consistently collide and switch positions. Also when colliding 
* with a boundry for some reason the robot goes back to its original place. 
* 
* Even after debugging for 8+ hours, I still do not know why these things are happening even though my logic for move forward 
* and check_collision all seem logically correct.
* 
* Also when stop command is called the robots do not stop, those have to be manually killed. (didn't figure out how to do this)
*/
			
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "simulator.h"

Environment    environment;  // The environment that contains all the robots

// Handle client requests coming in through the server socket.  This code should run
// indefinitiely.  It should repeatedly grab an incoming messages and process them. 
// The requests that may be handled are STOP, REGISTER, CHECK_COLLISION and STATUS_UPDATE.   
// Upon receiving a STOP request, the server should get ready to shut down but must
// first wait until all robot clients have been informed of the shutdown.   Then it 
// should exit gracefully.  
void *handleIncomingRequests(void *e) {
	char   online = 1;
	
	Environment	 *env = e; 
  	
	int                 serverSocket;
	struct sockaddr_in  serverAddr, clientAddr;
	int                 status, addrSize, bytesReceived;
	fd_set              readfds, writefds;
	unsigned char       buffer[80];
	unsigned char       response[80];

	//values stored in buffer
	unsigned short 		xHigh = 0;
	unsigned short 		xLow = 0;
	unsigned short 		yHigh = 0;
	unsigned short 		yLow = 0;
	unsigned short 		randDirection = 0;
	unsigned short 		randMagnitude = 0;

	//predicted new x/y values for forward
	float				newRobotX = 0;
	float				newRobotY = 0;

	//values recieved from response for status
	unsigned short 		xHighStatus = 0;
  	unsigned short      xLowStatus = 0;
	unsigned short  	yHighStatus = 0;
  	unsigned short     	yLowStatus = 0;
  	unsigned short    	randDirectionStatus = 0;
	unsigned short   	randMagnitudeStatus = 0;

	// ******************************************************** INITIALIZE THE SERVER ********************************************************
	
	// Create the server socket
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSocket < 0) {
		printf("*** SERVER ERROR: Could not open socket.\n");
		exit(-1);
	}

	// Setup the server address
	memset(&serverAddr, 0, sizeof(serverAddr)); // zeros the struct
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons((unsigned short) SERVER_PORT);

	// Bind the server socket
	status = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	if (status < 0) {
		printf("*** SERVER ERROR: Could not bind socket.\n");
		exit(-1);
	}

  	// Wait for clients now
	while (online) {
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);
		FD_ZERO(&writefds);
		FD_SET(serverSocket, &writefds);

		status = select(FD_SETSIZE, &readfds, &writefds, NULL, NULL);
		if (status == 0) {
			// Timeout occurred, no client ready
		} else if (status < 0) {
			printf("*** SERVER ERROR: Could not select socket.\n");
			exit(-1);
		} else {
			addrSize = sizeof(clientAddr);
			bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddr, &addrSize);
			if (bytesReceived > 0) {
				buffer[bytesReceived] = '\0';
			}
			
			// Client said to REGISTER
			if (buffer[0] == REGISTER) {
				++env->numRobots;
				if(env->numRobots < MAX_ROBOTS){
					//randomize location for robot
					for (int i=0; i<MAX_ROBOTS; i++){
						env->robots[env->numRobots-1].x = (float)((rand()/(double)RAND_MAX*(ENV_SIZE-ROBOT_RADIUS-ROBOT_RADIUS))+ROBOT_RADIUS);
						env->robots[env->numRobots-1].y = (float)((rand()/(double)RAND_MAX*(ENV_SIZE-ROBOT_RADIUS-ROBOT_RADIUS))+ROBOT_RADIUS);
						//check robot does not spawn on existing robot
						if(sqrt(pow((env->robots[env->numRobots-1].x-(env->robots[i].x)),2) + pow((env->robots[env->numRobots-1].y-(env->robots[i].y)),2)) > (2*ROBOT_RADIUS)) {
							break;
						}
					}
					printf("SERVER: id = %d x = %f\n", env->numRobots-1, env->robots[env->numRobots-1].x);
					printf("SERVER: id = %d y = %f\n", env->numRobots-1, env->robots[env->numRobots-1].y);

					//store high/low bytes for location
					xHigh = (unsigned short)env->robots[env->numRobots-1].x / 256;
					xLow =  (unsigned short)env->robots[env->numRobots-1].x % 256;
					yHigh = (unsigned short)env->robots[env->numRobots-1].y / 256;
					yLow =  (unsigned short)env->robots[env->numRobots-1].y % 256;

					//store direction/magnitude for direction
					randDirection = (unsigned short)(rand() % 2); //0 is positive & 1 is negative
					randMagnitude = (unsigned short)(rand()/(double)RAND_MAX*180);

					//add direction/magnitude to robot
					if (randDirection == 0){
						env->robots[env->numRobots-1].direction = randMagnitude;
					} else if (randDirection == 1) {
						env->robots[env->numRobots-1].direction = -randMagnitude;
					}
					printf("SERVER: id = %d direction = %d\n", env->numRobots-1, env->robots[env->numRobots-1].direction);

					
					//response
					response[0] = (unsigned char)OK;
					response[1] = (unsigned char)(env->numRobots-1); //robot ID
					response[2] = (unsigned char)xHigh;
					response[3] = (unsigned char)xLow;
					response[4] = (unsigned char)yHigh;
					response[5] = (unsigned char)yLow;
					response[6] = (unsigned char)randDirection;
					response[7] = (unsigned char)randMagnitude;
				} else {
					response[0] = NOT_OK;
				}

				// Respond to client
				sendto(serverSocket, response, sizeof(response), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
			}

			// Client said to STOP
			if (buffer[0] == STOP) {
				env->shutDown = 1;
				response[0] = LOST_CONTACT;
				break;
			}

			// Client said to CHECK_COLLISION
			if (buffer[0] == CHECK_COLLISION) {

				//shutdown robot
				if(environment.shutDown == 1) {
					response[0] = LOST_CONTACT;
					printf("SERVER: LOST_CONTACT\n");
					env->numRobots--;
				}

				//calculate new location
				newRobotX = env->robots[(unsigned short)buffer[1]].x+(ROBOT_SPEED*cos((double)env->robots[(unsigned short)buffer[1]].direction/180*PI));
				newRobotY = env->robots[(unsigned short)buffer[1]].y+(ROBOT_SPEED*sin((double)env->robots[(unsigned short)buffer[1]].direction/180*PI));

				//check collision with other robot
				for (int i=0; i<MAX_ROBOTS; i++){
					//do not compare to itself
					if (env->robots[i].x == env->robots[(unsigned short)buffer[1]].x){
						continue;
					}
					if(sqrt(pow((newRobotX-(env->robots[i].x)),2) + pow((newRobotY-(env->robots[i].y)),2)) <= (2*ROBOT_RADIUS)) {
						response[0] = NOT_OK_COLLIDE;
						// printf("SERVER: NOT_OK_COLLIDE\n");
						break;
					}
				}
				//check collision with boundry
				if((newRobotX <= ROBOT_RADIUS) || (newRobotY <= ROBOT_RADIUS) || (newRobotX >= ENV_SIZE-ROBOT_RADIUS) || (newRobotY >= ENV_SIZE-ROBOT_RADIUS)){
					response[0] = NOT_OK_BOUNDARY;
					// printf("SERVER: NOT_OK_BOUNDARY\n");
				} else {
					response[0] = OK;
					// printf("SERVER: OK\n");
				}

				// Respond to client
				sendto(serverSocket, response, sizeof(response), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
			}

			// Client said to STATUS_UPDATE
			if (buffer[0] == STATUS_UPDATE) {
				//set new values of robot
				xHighStatus = 		  (unsigned short)(buffer[2]);
				xLowStatus = 		  (unsigned short)(buffer[3]);
				yHighStatus = 		  (unsigned short)(buffer[4]);
				yLowStatus = 		  (unsigned short)(buffer[5]);
				randDirectionStatus = (unsigned short)(buffer[6]);
				randMagnitudeStatus = (unsigned short)(buffer[7]);

				env->robots[(unsigned short)buffer[1]].x = (float)((xHighStatus*256)+xLowStatus);
				env->robots[(unsigned short)buffer[1]].y = (float)((yHighStatus*256)+yLowStatus);

				if (randDirectionStatus == 0){
					env->robots[(unsigned short)buffer[1]].direction = (unsigned short)randMagnitudeStatus;
				} else if (randDirectionStatus == 1) {
					env->robots[(unsigned short)buffer[1]].direction = -(unsigned short)randMagnitudeStatus;
				}
			}		
		}
	}
  	// ... WRITE ANY CLEANUP CODE HERE ... //
  	close(serverSocket);
  	printf("SERVER: Shutting down.\n");
  	pthread_exit(NULL);
}


int main() {
	// So far, the environment is NOT shut down
	environment.shutDown = 0;
  
	// Set up the random seed
	srand(time(NULL));

	//create threads
	pthread_t t1, t2;

	Environment *envPtr = &environment;

	// Spawn an infinite loop to handle incoming requests and update the display
	pthread_create(&t1, NULL, handleIncomingRequests, envPtr);
 	pthread_create(&t2, NULL, redraw, envPtr);
	
	// Wait for the update and draw threads to complete
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
