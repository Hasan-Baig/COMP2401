/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: November 16 2020
 *
 */

typedef struct Obstacle {
  int x;
  int y;
  int w;
  int h;
} Obstacle;

typedef struct Vertex {
  int x;
  int y;
  struct Neighbour *list;           //list
  struct Neighbour *firstNeighbour; //head
  struct Neighbour *lastNeighbour;  //tail
  Obstacle *obstacle;
} Vertex;

typedef struct Neighbour {
  Vertex *vertex;
  struct Neighbour *next;
} Neighbour;

typedef struct Environment{
  struct Obstacle *obstacles;
  int numObstacles;
  struct Vertex *vertices;
  int numVertices;
} Environment;

int createVertices(Environment *env);
int createEdges(Environment *env);
int removeEdges(Environment *env);
void cleanupEverything(Environment *env);
