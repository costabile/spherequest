// hud.cpp
// contains functions relating to the drawing of the HUD
// Written by Jason Costabile, Ben Kybartas, Kaitlin Smith

#include <string>
#include "hud.h"
#include "player.h"
#include "challengelist.h"

//static int zen = 100; // Not sure if this is the best place for this...
//static int level = 1;
using namespace std;

HUD::HUD(){}

challengelist list = challengelist();

void HUD::printText(float x, float y, char *string, float r, float g, float b) //renders string on the screen at coords (x, y) in color (r, g, b)
{
	int len, i;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]); //if you don't like this font, here's a list of options: http://pyopengl.sourceforge.net/documentation/manual/glutBitmapCharacter.3GLUT.html
	}
}

void HUD::drawIntroText() {
	beginHudDrawing();

	printText(0.35, 1.3, "Use the arrow keys to move. Right-click for save/load menu.", 0.2, 0.5, 0.7);
	printText(0.1, 1.8, "Find the wise men and solve their challenges to ascend to higher planes!", 0.2, 0.6, 0.8);
	printText(0.50, 2.1, "You begin your journey in the Garden of the Blind", 0.2, 0.2, 0.2);
	
	endHudDrawing();
}

void HUD::drawQuestion(char * question, char * a, char * b, char * c) {

	beginHudDrawing();
	printText(0.10, 0.5, "Fiddle-di-Diddle, come solve my Riddle:", 0.1, 0.4, 0.6);
	printText(0.10, 1.0, question, 0.2, 0.5, 0.7);
	printText(0.23, 1.8, "F1:", 0.3, 0.6, 0.8);
	printText(0.23, 2.2, "F2:", 0.4, 0.7, 0.9);
	printText(0.23, 2.6, "F3:", 0.5, 0.8, 1.0);
	printText(0.35, 1.8, a, 0.3, 0.6, 0.8);
	printText(0.35, 2.2, b, 0.4, 0.7, 0.9);
	printText(0.35, 2.6, c, 0.5, 0.8, 1.0);
	
	endHudDrawing();
}
void HUD::printPlayAgainMsg(bool isWin) {
	beginHudDrawing();

	const float msgX = 1.15;
	const float msgY = 1.3;
	if (isWin) {
		printText(msgX-0.35, msgY, "You have acheived Zen, be at peace!", 0.2, 0.5, 0.7);
		printText(msgX-0.35, msgY + .5, "Would you like to ascend again? (Y/N)", 0.2, 0.6, 0.8);
	} else {
		printText(msgX-0.40, msgY, "Your mind is lost to mortal perversions.", 0.8, 0.1, 0.1);
		printText(msgX-0.35, msgY + .5, "Clear your mind, and try again? (Y/N)", 0.8, 0.3, 0.3);
	}

	endHudDrawing();
}

void HUD::printSaveLoadFeedback(bool isSave, bool isSuccessful) { //print success/failure message for a save/load operation
	//isSave = true if it's for a save operation, false if it's a load
	beginHudDrawing();

	const float msgX = 0.06;
	const float msgY = 2.8;
	const float msgRGB[3] = {0.8, 0.1, 0.1};
	if (isSave) {
		if (isSuccessful) {
			printText(msgX, msgY, "Game saved", msgRGB[0], msgRGB[1], msgRGB[2]);
		} else {
			printText(msgX, msgY, "Game save failed", msgRGB[0], msgRGB[1], msgRGB[2]);	//remove the () part
		}
	} else {
		if (isSuccessful) {
			printText(msgX, msgY, "Game loaded", msgRGB[0], msgRGB[1], msgRGB[2]);
		} else {
			printText(msgX, msgY, "Game load failed", msgRGB[0], msgRGB[1], msgRGB[2]);	//remove the () part
		}
	}

	endHudDrawing();
}

void HUD::printQuestionFeedback(bool isCorrect) {
	beginHudDrawing();
	const float msgX = 0.85;
	const float msgY = 1.4;
	const float msgRGB[3] = {0.8, 0.8, 0.8};
	if (isCorrect) {
		printText(msgX, msgY, "Your mind ascends closer to Zen.", msgRGB[0], msgRGB[1], msgRGB[2]);
	} else {
		printText(msgX-.2, msgY, "Your mind is heavy with doubt. Try again.", msgRGB[0], msgRGB[1], msgRGB[2]);
	}
	endHudDrawing();
}

void HUD::printLevelName(int level){
	beginHudDrawing();

	if (level == 2){
		printText(0.57, 1.7, "You meander into the Courtyard of the Wanderer.", 0.4, 0.4, 0.4);
	}
	if (level == 3){
		printText(0.60, 1.7, "You faithfully enter the Temple of the Follower.", 0.6, 0.6, 0.6);
	}
	if (level == 4){
		printText(0.60, 1.7, "You fearfully begin the Trial of the Unworthy.", 0.8, 0.8, 0.8);
	}
	if (level == 5){
		printText(0.65, 1.7, "You wisely traverse the Path of the Sages.", 1, 1, 1);
	}
	if (level == 6){
		printText(0.70, 1.7, "You are cheating if you are reading this.", 1, 1, 1);
	}
	
	endHudDrawing();
}
void HUD::drawHUD() { //draws a 2D overlay
	beginHudDrawing();
	
	//print text on HUD here--------
	const float hudTextY = 0.15;
	printText(0.05, hudTextY, "Zen: %", 0.1, 0.1, 0.1);	//print Zen label
	char zenStr[4];
	_itoa(zen, zenStr, 10); // itoa converts an integer to a string
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

	endHudDrawing();
}

void HUD::drawIntroScreen() {		//draws a quad to display the intro screen on. Texturing is done by calling function.
	beginHudDrawing();

	glBegin(GL_QUADS);
	
		glTexCoord2d(0.0, 0.0); glVertex2f(0.0, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex2f(3.0, 0.0);
		glTexCoord2d(1.0, 1.0); glVertex2f(3.0, 3.0);
		glTexCoord2d(0.0, 1.0); glVertex2f(0.0, 3.0);

	glEnd();

	endHudDrawing();
}

void HUD::beginHudDrawing() {	//sets up OpenGL for drawing 2d overlay stuff. Always call this before drawing HUD stuff.
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();							//push projection matrix
	glLoadIdentity();
	glOrtho(0.0, 3.0, 3.0, 0.0, -1, 1);		//set ortho mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();							//push modelview matrix
	glLoadIdentity();
}

void HUD::endHudDrawing() {		//Always call this when finished drawing HUD stuff.
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();									//pop projection matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();									//pop modelview matrix

	glEnable(GL_CULL_FACE);
}