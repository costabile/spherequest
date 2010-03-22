// hud.cpp
// contains functions relating to the drawing of the HUD

#include "hud.h"
#include "player.h"

//static int zen = 100; // Not sure if this is the best place for this...
//static int level = 1;

HUD::HUD(){}

void HUD::printText(float x, float y, char *string, float r, float g, float b)		//renders string on the screen at coords (x, y) in color (r, g, b)
{
	int len, i;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);			//if you don't like this font, here's a list of options: http://pyopengl.sourceforge.net/documentation/manual/glutBitmapCharacter.3GLUT.html
	}
}

void HUD::drawIntroText() {
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();							//push projection matrix
	glLoadIdentity();
	glOrtho(0.0, 3.0, 3.0, 0.0, -1, 1);		//set ortho mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();							//push modelview matrix
	glLoadIdentity();

	printText(0.9, 1.3, "Use the arrow keys to move.", 0.2, 0.5, 0.7);
	printText(0.1, 1.8, "Find the wise men and solve their challenges to ascend to higher planes!", 0.2, 0.6, 0.8);

	printText(0.3, 2.2, "(Challenges not implemented yet, but please explore the level)", 0.2, 0.6, 0.8);	//just so they know. Remove this later.

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();									//pop projection matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();									//pop modelview matrix

	glEnable(GL_CULL_FACE);
}

void HUD::drawHUD() {		//draws a 2D overlay

	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();							//push projection matrix
	glLoadIdentity();
	glOrtho(0.0, 3.0, 3.0, 0.0, -1, 1);		//set ortho mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();							//push modelview matrix
	glLoadIdentity();

	//print text on HUD here--------
	const float hudTextY = 0.15;
	printText(0.05, hudTextY, "Zen: %", 0.1, 0.1, 0.1);	//print Zen label
	char zenStr[4];
	_itoa(zen, zenStr, 10);
	printText(0.31, hudTextY, zenStr, 0.6, 0.2, 0.2);	//print amount of Zen
	printText(0.94, hudTextY, "SphereQuest(TM)", 0.8, 0.3, 0.3);	//print Title
	printText(1.95, hudTextY, "Plane of Consciousness:", 0.3, 0.3, 0.3);		//print level label
	char planeStr[3];
	_itoa(level, planeStr, 10);
	printText(2.91, hudTextY, planeStr, 0.6, 0.2, 0.2);		//print level/plane #
	//end text

	glBegin(GL_QUADS);						//draw HUD bar
	//glColor4f(0.95, 0.95, 0.95, 1.0);
	glColor3f(0.8, 0.8, 1);
	glVertex2f(0.0, 0.0);
	glColor3f(0.8, 0.8, 1);
	glVertex2f(6.0, 0.0);
	glColor3f(1, 1, 1);
	glVertex2f(6.0, 0.2);
	glColor3f(1, 1, 1);
	glVertex2f(0.0, 0.2);
	glEnd();


	glMatrixMode( GL_PROJECTION );
	glPopMatrix();									//pop projection matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();									//pop modelview matrix

	glEnable(GL_CULL_FACE);
}