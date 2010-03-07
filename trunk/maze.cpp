// maze.h
// contains functions and attributes related to the maze/level

#include "maze.h"

int mazeLayout[6][6] = {		//initialized to first-level layout
	{3, 2, 2, 3, 2, 3},
	{1, 0, 0, 1, 5, 1},
	{1, 0, 2, 3, 0, 1},
	{1, 0, 0, 0, 0, 3},
	{1, 5, 4, 1, 0, 1},
	{3, 2, 2, 3, 4, 3}
	};

//0 = empty
//1 = x wall
//2 = z wall
//3 = temple
//4 = wise man
//5 = tree

maze::maze(){};


void maze::generateMaze()
{
	//Empty Maze Matrix
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			mazeLayout[i][j] = 0;
		}
	} 
	//Define the following maze (1 = wall, 2 = Wise Man!)
	// 1 1 0 1 1 1
	// 1 0 0 1 0 1
	// 1 0 1 0 0 1
	// 1 0 0 0 1 1
	// 1 0 1 0 0 1
	// 1 1 1 1 2 1

	//The following is an EXTREMELY brute force way of defining a map.
	//The maps will later be read from a file!
	for (int i = 0; i < 6; i++)
	{
		mazeLayout[i][0] = 1;
		mazeLayout[i][5] = 1;
	}

	for (int j = 0; j < 6; j++)
	{
		mazeLayout[0][j] = 1;
		mazeLayout[5][j] = 1;
	}

	mazeLayout[0][2] = 0;
	mazeLayout[5][4] = 2;
	mazeLayout[1][3] = 1;
	mazeLayout[2][2] = 1;
	mazeLayout[3][4] = 1;
	mazeLayout[4][2] = 1;
}


//A quick call to see what the value of maze is
int maze::checkMaze(int row, int column)
{
	return mazeLayout[row][column];
}

