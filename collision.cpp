// collision.cpp
// contains functions related to collision detection

#include "collision.h"
#include "hud.h"

#define TOLERANCE 0.01f			//the amount of error allowed to collide with certain objects

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

float collision::getUpperBound(float obstacleWidth, float tolerance) {	//finds the upper limit of a given obstacle within a cell.
	//the bounds of an obstacle can be made less strict (narrower) with a positive tolerance, or more strict (wider) with a negative tolerance
	return CELL_SIDE / 2.0 + obstacleWidth / 2.0 - tolerance;
}

float collision::getLowerBound(float obstacleWidth, float tolerance) {	//finds the lower limit of a given obstacle within a cell.
	//the bounds of an obstacle can be made less strict (narrower) with a positive tolerance, or more strict (wider) with a negative tolerance
	return CELL_SIDE / 2.0 - obstacleWidth / 2.0 + tolerance;
}

bool collision::checkPointCollision(float movX, float movZ) {		//checks if a point at movX, movZ would be colliding with an obstacle
	bool isCollision = false;

	int gridX = getGridPositionX(movX);
	int gridZ = getGridPositionZ(movZ);

	movX = movX + CELL_SIDE * (MAP_SIDE / 2);		//shift coordinates so that they are in the positive plane
	movZ = movZ + CELL_SIDE * (MAP_SIDE / 2);

	float upper_bound;
	float lower_bound;

	switch (levelMaze->checkMaze(gridX, gridZ)) {		//check what object occupies this cell, and plan collision geometry accordingly
		case 1:		//Xwall
			upper_bound = getUpperBound(20.0f, TOLERANCE);
			lower_bound = getLowerBound(20.0f, TOLERANCE);
			if ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) {
				isCollision = true;
			}
			break;
		case 2:		//Zwall
			upper_bound = getUpperBound(20.0f, TOLERANCE);
			lower_bound = getLowerBound(20.0f, TOLERANCE);
			if ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound)) {
				isCollision = true;
			}
			break;
		case 3:		//temple
			upper_bound = getUpperBound(40.0f, TOLERANCE);
			lower_bound = getLowerBound(40.0f, TOLERANCE);
			if ( ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound))
				&& ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) ) {	//if x and z of point is in the temple
				isCollision = true;
			}
			break;
		case 4:		//wiseMan
			upper_bound = getUpperBound(0.9f, -75.0 * TOLERANCE);	//wisemen need high negative tolerance because they're small, i.e. the sphere tends to jump through them
			lower_bound = getLowerBound(0.9f, -75.0 * TOLERANCE);
			//cout << upper_bound << "," << lower_bound << endl;
			//cout << (fmod(movX, (float)CELL_SIDE)) << ":" << (fmod(movZ, (float)CELL_SIDE)) << endl;
			if ( ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound))
				&& ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) ) {	//if x and z of point is in the wise man body
				isCollision = true;
			}
			break;
		case 5:		//tree
			upper_bound = getUpperBound(3.5f, -15 * TOLERANCE);
			lower_bound = getLowerBound(3.5f, -15 * TOLERANCE);
			if ( ((fmod(movX, (float)CELL_SIDE) < upper_bound) && (fmod(movX, (float)CELL_SIDE) > lower_bound))
				&& ((fmod(movZ, (float)CELL_SIDE) < upper_bound) && (fmod(movZ, (float)CELL_SIDE) > lower_bound)) ) {	//if x and z of point is in the tree
				isCollision = true;
			}
			break;
		default:	//no obstacles/empty cell.  No collision.
			break;
	}
	if (!isCollision) {
		if ((gridX >= MAP_SIDE) || (gridX < 0) || (gridZ >= MAP_SIDE) || (gridZ < 0)) {
			//cout << gridX << "//" << gridZ << endl;
			isCollision = true;		//don't let the player leave the bounds of the grid
		}
	}

	return isCollision;
}

bool collision::checkCollision(float movX, float movY, float movZ, float objectRadius) { //checks whether moving to movX, movY, movZ would be a collision
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
	//centre of object (to help prevent "jumping through" small/thin obstacles):
	isCollision = (isCollision || checkPointCollision(movX, movZ));

	return isCollision;	
}