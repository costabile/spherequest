// collision.h
// contains functions related to collision detection

#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>		//for debugging. Remove when done.
#include <stdlib.h>
#include <glut.h>
#include <cmath>

#include "maze.h"

using namespace std;	//debugging

class collision {
private:
	bool checkPointCollision(float movX, float movZ);
	float getUpperBound(float obstacleWidth, float tolerance);
	float getLowerBound(float obstacleWidth, float tolerance);

public:
	collision(maze *mazeObj); // Constructor
	~collision(){}; // Destructor
	int getGridPositionX(float locX);
	int getGridPositionZ(float locZ);
	bool checkCollision(float movX, float movY, float movZ, float objectRadius);
};

#endif