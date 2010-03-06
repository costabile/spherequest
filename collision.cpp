// collision.cpp
// contains functions related to collision detection

#include "collision.h"

static maze *levelMaze;

collision::collision(maze *mazeObj){
	levelMaze = mazeObj;
}

int collision::getGridPositionX(float locX) {		//given specific coordinates, returns which map X grid cell they correspond to
	return (locX + (MAP_SIDE/2 * CELL_SIDE)) / CELL_SIDE;		//x grid index
}

int collision::getGridPositionZ(float locZ) {		//returns z grid coordinate
	return (locZ + (MAP_SIDE/2 * CELL_SIDE)) / CELL_SIDE;		//z grid index
}

bool collision::checkCollision(float movX, float movY, float movZ, float objectRadius) {		//checks whether moving to movX, movY, movZ would be a collision
	bool isCollision = false;

	int gridX = getGridPositionX(movX);
	int gridZ = getGridPositionZ(movZ);

	if (/*(levelMaze->checkMaze(gridX, gridZ) == 1) || (levelMaze->checkMaze(gridX, gridZ) == 2) || */(levelMaze->checkMaze(gridX, gridZ) == 3)/* or is out of bounds*/) {
		isCollision = true;
	}

	return isCollision;	
}