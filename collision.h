// collision.h
// contains functions related to collision detection

#ifndef COLLISION_H
#define COLLISION_H

#include <stdlib.h>
#include <glut.h>
#include <cmath>

#include "maze.h"

class collision {
private:
	bool checkPointCollision(float movX, float movZ);

public:
	collision(maze *mazeObj); // Constructor
	~collision(){}; // Destructor
	int getGridPositionX(float locX);
	int getGridPositionZ(float locZ);
	bool checkCollision(float movX, float movY, float movZ, float objectRadius);
};

#endif