/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: October 10 2020
 */

#include <stdio.h>
#include <string.h>

//Function prototypes (forward reference)
void printEncodedMaze(int maze[], unsigned char dimensions);
void printEncodedMazeWithPath(int maze[], int path[], unsigned char dimensions);
void encode8by8Maze(char inputMaze[8][8], int encodedMaze[8]);
void merge8by8Mazes(int topLeft[8], int topRight[8], int bottomLeft[8], int bottomRight[8], int finalMaze[14]);
char isPathInMaze(int maze[], int path[], unsigned char dimensions);

int main() {

    //original 32x32 maze
    int mazeArray[32] = {
        -1, -2146430423, -1109928277, -1525661045, -1602496045, -1358950569,
        -1451509759, -1927115297, -1578180479, -1354977603, -1476294999,
        -2084818261, -1163749757, -1964319689, -1341614619, -1750141691,
        -1256060947, -1515522751, -1204136611, -1955867371, -1190652827,
        -1553272339, -1100839163, -1999963019, -631368865, -1882138047,
        -1172944163, -1412279803, -1567107339, -164346691, -2138762879, -1
    };

    //path in 32x32 maze
    int pathArray[32] = {
        0, 0, 0, 0, 12, 8, 56, 32, 8032, 4416, 134115648, 67354944, 67109184,
        67109312, 133169152, 1048576, 1835008, 262144, 262144, 262144, 458752,
        65536, 65536, 65536, 65536, 983040, 67633152, 67633152, 201850880,
        164102144, 259522560, 0
    };

    //8x8 mazes as 2d arrays
    char inputMazeOne[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,1},
        {1,0,1,0,1,1,0,1},
        {1,0,1,0,0,0,0,1},
        {1,0,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,1},
        {1,1,1,1,1,1,1,1}};

    char inputMazeTwo[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,1},
        {1,1,1,1,0,1,0,1},
        {1,0,0,1,0,1,1,1},
        {1,1,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1},
        {1,0,0,0,0,1,0,1},
        {1,1,1,1,1,1,1,1}};

    char inputMazeThree[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,1,0,0,0,1,1},
        {1,0,1,0,1,0,0,1},
        {1,0,1,0,1,0,1,1},
        {1,0,1,0,1,0,1,1},
        {1,0,1,0,1,0,1,1},
        {1,0,0,0,1,0,0,1},
        {1,1,1,1,1,1,1,1}};

    char inputMazeFour[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,1,0,1,0,1,1},
        {1,0,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,1},
        {1,1,1,0,1,0,0,1},
        {1,0,0,0,0,0,1,1},
        {1,1,0,1,1,0,0,1},
        {1,1,1,1,1,1,1,1}};

    //encoded 8x8 mazes into array of int
    int encodedMazeOne[8];
    int encodedMazeTwo[8];
    int encodedMazeThree[8];
    int encodedMazeFour[8];

    //final 14x14 maze
    int finalMaze[14];

    //array populated with encoded the 8x8 mazes
    int *encodedMazeArrays[4] = {
       encodedMazeOne,
       encodedMazeTwo,
       encodedMazeThree,
       encodedMazeFour
    };

    //stores each combination of each 14x14 maze
    int combinationMaze[14];

    //encoded path that should fit 8 of the 256 combinations of encoded mazes
    int encodedPath[14] = {
        0, 4096, 4096, 4096, 4096, 7936, 256, 256, 448, 112, 16, 28, 6, 0
    };

    //Part 2 - Print 32x32 maze
    printf("**********************************************\n\n");
    printf("Part 2\n\n");
    printEncodedMaze(mazeArray, 32);

    //Part 3 - Print 32x32 maze with path
    printf("**********************************************\n\n");
    printf("Part 3\n\n");
    printEncodedMazeWithPath(mazeArray, pathArray, 32);

    //Part 7a - Determine if path fits in 32x32 maze
    if (isPathInMaze(mazeArray, pathArray, 32)){
        printf("\nPart 7a: The path fits in the 32x32 maze.\n\n");
    }

    //Part4 - Encode and print 8x8 mazes
    printf("**********************************************\n\n");
    printf("Part 4\n\n");
    encode8by8Maze(inputMazeOne, encodedMazeOne);
    encode8by8Maze(inputMazeTwo, encodedMazeTwo);
    encode8by8Maze(inputMazeThree, encodedMazeThree);
    encode8by8Maze(inputMazeFour, encodedMazeFour);

    //Part 5 - Merge and print 8x8 mazes into a 14x14 maze
    printf("**********************************************\n\n");
    printf("Part 5\n\n");
    merge8by8Mazes(encodedMazeOne, encodedMazeTwo, encodedMazeThree, encodedMazeFour, finalMaze);

    //Part 6 - Print all 256 combinations 14x14 mazes
    printf("**********************************************\n\n");
    printf("Part 6\n\n");

    //iteratively prints all 256 mazes from 1,1,1,1 to 4,4,4,4
    for (int topL = 0; topL < 4; ++topL){
        for (int topR = 0; topR < 4; ++topR){
            for (int bottomL = 0; bottomL < 4; ++bottomL){
                for (int bottomR = 0; bottomR < 4; ++bottomR){
                    printf("Combination: %d, %d, %d, %d\n", topL+1, topR+1, bottomL+1, bottomR+1);
                    merge8by8Mazes(encodedMazeArrays[topL], encodedMazeArrays[topR], encodedMazeArrays[bottomL], encodedMazeArrays[bottomR], combinationMaze);

                    //Part 7b - Determine if encodedPath fits in any 14x14 maze combo
                    if (isPathInMaze(combinationMaze, encodedPath, 14)){
                        printf("Part 7b: The path fits in the 14x14 maze above, combination: %d, %d, %d, %d.\n\n", topL+1, topR+1, bottomL+1, bottomR+1);
                    }
                }
            }
        }
    }

    printf("**********************************************\n");
    return 0;
}

/*************************************************************
 * Purpose: read each bit of array of int to determine wall or empty space in maze
 * Input: maze and dimensions of maze
 * Output: printed 32x32 maze
 *************************************************************/
void printEncodedMaze(int maze[], unsigned char dimensions){
    for(char i=0; i<dimensions; i++){
        for(int j=(dimensions-1); j>=0; j--){
            unsigned int mask = (1<<j);
            if((maze[i] & mask) > 0){
                printf("%c", '@');
            } else {
                printf("%s", " ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*************************************************************
 * Purpose: read each bit of array of int to determine wall, empty space or path in maze
 * Input: maze, path, and dimensions of maze
 * Output: printed 32x32 maze with path
 *************************************************************/
void printEncodedMazeWithPath(int maze[], int path[], unsigned char dimensions){
    for(char i=0; i<dimensions; i++){
        for(int j=(dimensions-1); j>=0; j--){
            unsigned int mask = (1<<j);
            if((maze[i] & mask) > 0){
                printf("%c", '@');
            } else if((path[i] & mask) > 0){
                printf("%c", '.');
            } else {
                printf("%c", 32);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*************************************************************
 * Purpose: convert 2d array into single array of int
 * Input: 2d array of 1/0 and array to be inserted into
 * Output: printed 8x8 maze
 *************************************************************/
void encode8by8Maze(char inputMaze[8][8], int encodedMaze[8]){
    for(char i=0; i<8; i++){
        unsigned int mask = 0;
        for(char j=0; j<8; j++){
            mask |= (inputMaze[i][j]<<(7-j));
        }
        encodedMaze[i] = mask;
    }
    printEncodedMaze(encodedMaze, 8);
}

/*************************************************************
 * Purpose: merge four 8x8 mazes into a 14x14 maze while discarding inner rows/columns
 * Input: each encoded 8x8 maze array and 14x14 maze array to be inserted into
 * Output: printed 14x14 maze
 *************************************************************/
void merge8by8Mazes(int topLeft[8], int topRight[8], int bottomLeft[8], int bottomRight[8], int finalMaze[14]){
    unsigned char bitVal;
    int i,j;

    for(i=0; i<14; i++){
        unsigned int mask = 0;

        //left 7 columns of finalMaze
        for(j=7; j>0; --j){

            //top left
            if (i<7){
                bitVal = (topLeft[i] & (1<<j)) >> j;
                mask |= (bitVal<<(6+j));
            }
            //bottom left
            else {
                bitVal = (bottomLeft[i-6] & (1<<j)) >> j;
                mask |= (bitVal<<(6+j));
            }
        }

        //right 7 columns of finalMaze
        for(j=13; j>6; --j){

            //top right
            if (i<7){
                bitVal = (topRight[i] & (1<<(j-7))) >> (j-7);
                mask |= (bitVal<<(j-7));
            }
            //bottom right
            else {
                bitVal = (bottomRight[i-6] & (1<<(j-7))) >> (j-7);
                mask |= (bitVal<<(j-7));
            }
        }
        finalMaze[i] = mask;
    }

    printEncodedMaze(finalMaze, 14);
}

/*************************************************************
 * Purpose: check if path fits in maze without landing on walls
 * Input: maze, path and dimensions of maze
 * Output: 1 if true or 0 if false
 *************************************************************/
char isPathInMaze(int maze[], int path[], unsigned char dimensions){
    for(char i=0; i<dimensions; i++){
        for(int j=(dimensions-1); j>=0; j--){
            unsigned int mask = (1<<j);
            //maze wall
            if((maze[i] & mask) > 0){
                //path on wall
                if((path[i] & mask) > 0){
                    return 0;
                }
            }
        }
    }
    return 1;
}
