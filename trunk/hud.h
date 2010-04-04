// hud.h
// contains function primitives related to drawing the hud

#ifndef HUD_H
#define HUD_H

#include <stdlib.h>
#include <glut.h>
#include <string.h>

#include <iostream>

#include "player.h"

class HUD {
public:
	HUD(); // Constructor
	~HUD(){}; // Destructor
	void printText(float x, float y, char *string, float r, float g, float b);
	void drawIntroText();
	void drawQuestion(char * q, char * a, char * b, char * c);
	void printPlayAgainMsg(bool isWin);
	void printSaveLoadFeedback(bool isSave, bool isSuccessful);
	void drawHUD();
private:
	void beginHudDrawing();
	void endHudDrawing();
};

#endif