// collision.h
// contains functions related to collision detection

#ifndef COLLISION_H
#define COLLISION_H

#include <stdlib.h>
#include <glut.h>

#include "maze.h"

class collision {
public:
	collision(maze *mazeObj); // Constructor
	~collision(){}; // Destructor
	int getGridPositionX(float locX);
	int getGridPositionZ(float locZ);
	bool checkCollision(float movX, float movY, float movZ, float objectRadius);
};

#endif