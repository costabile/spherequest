// collision.cpp
// contains functions related to collision detection

#include "collision.h"
#include "hud.h"

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

bool collision::checkPointCollision(float movX, float movZ) {		//checks if a point at movX, movZ would be colliding with an obstacle
	//does not yet work for wise men or trees.  Works for some Xwalls and Zwalls but does not work for others.  Work in progress.
	bool isCollision = false;
	
	int gridX = getGridPositionX(movX);
	int gridZ = getGridPositionZ(movZ);
	float upper_bound;
	float lower_bound;

	switch (levelMaze->checkMaze(gridX, gridZ)) {		//check what object occupies this cell, and plan collision geometry accordingly
		case 1:		//Xwall
			upper_bound = CELL_SIDE / 2.0 + 20.0f / 2.0;
			lower_bound = CELL_SIDE / 2.0 - 20.0f / 2.0;
			if ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) {
				isCollision = true;
			}
			break;
		case 2:		//Zwall
			upper_bound = CELL_SIDE / 2.0 + 20.0f / 2.0;
			lower_bound = CELL_SIDE / 2.0 - 20.0f / 2.0;
			if ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound)) {
				isCollision = true;
			}
			break;
		case 3:		//temple
			isCollision = true;		//temples occupy entire cell.  Always collision.
			break;
		case 4:		//wiseMan
			upper_bound = CELL_SIDE / 2.0 + 0.6f / 2.0;
			lower_bound = CELL_SIDE / 2.0 - 0.6f / 2.0;
			if ( ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound))
				&& ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) ) {	//if x and z of point is in the wise man body
				isCollision = true;
			}
			break;
		case 5:		//tree
			upper_bound = CELL_SIDE / 2.0 + 3.0f / 2.0;
			lower_bound = CELL_SIDE / 2.0 - 3.0f / 2.0;
			if ( ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound))
				&& ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) ) {	//if x and z of point is in the tree
				isCollision = true;
			}
			break;
		default:	//no obstacles/empty cell.  No collision.
			break;
	}

	//todo: or is out of bounds!

	return isCollision;
}

bool collision::checkCollision(float movX, float movY, float movZ, float objectRadius) {		//checks whether moving to movX, movY, movZ would be a collision
	bool isCollision = false;
	//checks the relevant faces of a "bounding box" around the object to see whether they would collide in the new position.

	//face 1:
	isCollision = checkPointCollision(movX + objectRadius, movZ);
	//face 2:
	isCollision = (isCollision || checkPointCollision(movX, movZ + objectRadius));
	//face 3:
	isCollision = (isCollision || checkPointCollision(movX - objectRadius, movZ));
	//face 4:
	isCollision = (isCollision || checkPointCollision(movX, movZ - objectRadius));

	return isCollision;	
}