// draw.h
// Contains functions relating to drawing commands.

#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <glut.h>

class draw {
public:
	draw(); // Constructor
	~draw(){}; // Destructor
	//void drawSphere();
	void drawTemples(GLuint roofTexture, GLuint redTexture);
	void drawXWall();
	void drawZWall();
	void drawTree();
	void drawWiseMen();
	void drawGround();
	void drawSkybox();
	void drawMtFuji();
};

#endif