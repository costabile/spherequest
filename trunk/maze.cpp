// maze.h
// contains functions and attributes related to the maze/level

#include "maze.h"

int mazeLayout[6][6];

using namespace std;

maze::maze(){};


//A quick call to see what the value of maze is
int maze::checkMaze(int row, int column){
	return mazeLayout[row][column];
}

void maze::changeLevel(int levelNum) {		//change map to the specified level	(calling function must manually set new sphere position, though)
	stringstream out;
	out << levelNum;
	string fileName = "mazes\\level" + out.str() + ".txt";
	readMaze(fileName.c_str());		//read in the level maze
	level = levelNum;
}

void maze::readMaze(const char* fileName){
	int tileID;

	ifstream openMaze(fileName); // Open the specified file

	if (openMaze.is_open()){ // If the file opened successfully, we can proceed.
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 6; j++){
				openMaze >> tileID;
				mazeLayout[i][j] = tileID;
			}
		}
		openMaze.close(); // We're done with the file, so close it now.
	} else {
		cout << "An error occured when trying to open the maze file.  Please try again." << endl;
	}
}