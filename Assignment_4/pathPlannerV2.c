/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: November 16 2020
 *
 */
 #include <stdio.h>
 #include <stdlib.h>

 #include "obstacles.h"

 #define NUM_OF_VERTEX_PER_OBSTACLE   4

 int createVertices(Environment *env){
   //each obstacle has 4 verticies
   env->numVertices = env->numObstacles*NUM_OF_VERTEX_PER_OBSTACLE;

   //dynamically allocate all verticies
   env->vertices = (Vertex *)malloc(env->numVertices*sizeof(Vertex));
   if (env->vertices == NULL) {
     printf("Memory allocation error\n");
     exit(0);
   }

   //counter for vertex
   int i = 0;

   //add vertex for each obstacle
   for (int j=0; j<env->numObstacles; j++) {

     //top left vertex
     env->vertices[i].x = env->obstacles[j].x;
     env->vertices[i].y = env->obstacles[j].y;
     env->vertices[i].obstacle = &env->obstacles[j];

     //top right vertex
     env->vertices[i+1].x = env->obstacles[j].x + env->obstacles[j].w;
     env->vertices[i+1].y = env->obstacles[j].y;
     env->vertices[i+1].obstacle = &env->obstacles[j];

     //bottom left vertex
     env->vertices[i+2].x = env->obstacles[j].x;
     env->vertices[i+2].y = env->obstacles[j].y - env->obstacles[j].h;
     env->vertices[i+2].obstacle = &env->obstacles[j];

     //bottom right vertex
     env->vertices[i+3].x = env->obstacles[j].x + env->obstacles[j].w;
     env->vertices[i+3].y = env->obstacles[j].y - env->obstacles[j].h;
     env->vertices[i+3].obstacle = &env->obstacles[j];

     i=i+4;
   }
   return env->numVertices;
}

int createEdges(Environment *env){
  for (int i=0; i<env->numVertices; i++) {
    Neighbour *head = env->vertices[i].list;
    Neighbour *tail = env->vertices[i].list;
    Neighbour *currNode = head;
    Neighbour *prevNode = NULL;

    for (int j=0; j<env->numVertices; j++) {

      //prevent self loop
      if ((env->vertices[i].x == env->vertices[j].x)&&(env->vertices[i].y == env->vertices[j].y)){
        continue;
      }

      Neighbour *newNode;

      // Create the new node
      newNode = (struct Neighbour *)malloc(sizeof(struct Neighbour));
      if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(0);
      }
      newNode->vertex = &env->vertices[j];
      newNode->next = NULL;      

      // If prevNode is NULL, then this is the first position in the list.
      if (prevNode == NULL)
        head = newNode;
      else
        prevNode->next = newNode;

      newNode->next = currNode; // Connect new node to rest of the list
    }
    env->vertices[i].firstNeighbour = head;
    env->vertices[i].lastNeighbour = tail;
  }
  int numEdges = ((4*env->numObstacles)*((4*env->numObstacles)-1));
  return numEdges;
}

int removeEdges(Environment *env){

  float uA, uB;
  int x1, x2, x3, x4;
  int y1, y2, y3, y4;
  int counter = 0; //edges removed

  //loop through all verticies
  for (int i=0; i<env->numVertices; i++) {
    x1 = env->vertices[i].x;
    y1 = env->vertices[i].y;
    //loop through all neighbours
    Neighbour *currNode = env->vertices[i].firstNeighbour;
    while (currNode != NULL) {
      x2 = currNode->vertex->x;
      y2 = currNode->vertex->y;

      //loop through all other verticies
      for (int j=0; j<env->numVertices; j++) {
        x3 = env->vertices[j].x;
        y3 = env->vertices[j].y;
        //loop through all other neighbours
        Neighbour *curr2Node = env->vertices[j].firstNeighbour;
        while (curr2Node != NULL) {
          x4 = curr2Node->vertex->x;
          y4 = curr2Node->vertex->y;

          uA = (((x4-x3)*(y1-y3))-((y4-y3)*(x1-x3)))/(((y4-y3)*(x2-x1))-((x4-x3)*(y2-y1)));
          uB = (((x2-x1)*(y1-y3))-((y2-y1)*(x1-x3)))/(((y4-y3)*(x2-x1))-((x4-x3)*(y2-y1)));

          //intersect or on same obstacle
          if (((uA > 0)&&(uA < 1))&&((uB > 0)&&(uB < 1)) || (env->vertices[i].obstacle == env->vertices[j].obstacle)) {
            free(currNode);
            free(curr2Node);
            counter++;
          }
          curr2Node = curr2Node->next;
        }
      }
      currNode = currNode->next;
    }
  }

  int numEdges = ((4*env->numObstacles)*((4*env->numObstacles)-1));
  return (numEdges-counter);
}

// Free all items in environment
void cleanupEverything(Environment *env){
  //free neighbours
  for (int i=0; i<env->numVertices; i++) {
    Neighbour *currNode = env->vertices[i].firstNeighbour;
    Neighbour *nextNode;
    while (currNode != NULL) {
      nextNode = currNode->next;
      free(currNode->vertex);
      free(currNode);
      currNode = nextNode;
    }
  }
  //free vertices
  free(env->vertices);
  //free obstacles
  free(env->obstacles);
}
