//SphereQuest
//Game Project for SW Eng 3GC3
//Written by Jason Costabile, Ben Kybartas, Kaitlin Smith

#include <stdlib.h>
#include <math.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <glut.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <direct.h>

#include "challengelist.h"
#include "hud.h"
#include "player.h"
#include "collision.h"
#include "maze.h"
#include "draw.h"

#define PI 3.14159265358979323846
#define SPHERE_RAD 2.0f				//sphere radius
#define CAM_DIST 20.0				//distance camera keeps from sphere
#define TURN_ANGLE PI/128.0f		//angle to turn camera on keypress
#define MOVE_SIZE 0.4f				//base sphere movement distance
#define ROT_ANG PI/64.0f			//angle to rotate sphere by each time it moves
#define DECEL 0.01f					//the rate at which the sphere's movement slows when the user is not pressing forward or back
#define ROT_DECEL TURN_ANGLE/5.5	//rate at which rotation slows when left or right are not pressed
#define COLLISION_SPACING 0.2f		//the max distance the sphere might be from an obstacle when it stops moving.  Smaller = more precise collisions.
#define ZEN_PENALTY 20				//the amount of zen you lose for a wrong answer
#define WIN_LEVEL 6				//the level you must reach to win

//window dimensions:
#define WIN_WIDTH 640
#define WIN_HEIGHT 360

//sphere position (initial = -60, 0+radius, -9.5):
GLfloat spherePosX = -120;
GLfloat spherePosY = 0.0 + SPHERE_RAD;
GLfloat spherePosZ = 0;

int currentlevel = 1;

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;		//camera coords

//Set up the list of challenges
challengelist challenges = challengelist();

//current sphere velocity:
float sphForwardVel = 0.0f;
float sphRotVel = 0.0f;	//rotating speed

/*
 For Animation Purposes!
*/
float anix = 0;
float aniPI = 3.1415926535897932384626433832795;

//for movement control:
bool dirBtnDown = false;
bool rotBtnDown = false;
//sphere rotation:
int sphereRotX = 0.0;
int sphereRotY = 0.0;
int sphereRotZ = 0.0;
float sphereRotAng = 0.0;

bool dev_mode = false;		//false = off.  Displays the layout of the grid.  'j' to toggle
bool question_mode = false;
bool intro_mode = true;
int playAgainMode = 0;		//0=off, 1=show win message, 2=show loss message. When on, it displays the "Play again" message and allows the user to press Y to play again or N to quit
int moveCount = 0;			//counts the number of sphere movements
int showSaveLoadMsg = 0;	//0 = don't show a save/load msg, 1=save success, 2=save fail, 3=load success, 4=load fail
int showQuestionMsg = 0;
int moveCountMsgMark = 0;	//used to determine when to stop displaying save/load messages
int moveCountQstnMark = 0;	//determining how long to display Question feedback
int difficulty_mod = 1;		//modifies the amount of zen you lose for an incorrect answer (based on difficulty mode chosen)

GLuint grassTexture;
GLuint skyboxTexture;
GLuint sphereTexture;
GLuint brickTexture;
GLuint roofTexture;
GLuint redTexture;
GLuint treeTexture;
GLuint introScreen;

static HUD *hud = new HUD();  // Create a HUD object
static maze *mazeObj = new maze();		//create maze!
static collision *collisions = new collision(mazeObj);		//create a new collision-checker!
static draw *drawing = new draw(); // Create a drawing object

//load texture. Based on tutorial found at http://www.swiftless.com/tutorials/opengl/texture_under_windows.html
GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );

	glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );
    return texture;
}

void FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture );
}

void moveCamera(float xDif, float yDif, float zDif) {		//moves the camera to xDif, yDif, zDif (always looking at sphere)
	
	glLoadIdentity();
	gluLookAt(xDif, yDif, zDif, 
		spherePosX, spherePosY, spherePosZ,		//always look at sphere
		0.0f,1.0f,0.0f);
}

void changeSize(int w, int h)
{
	//prevent change in window dimensions:
	w = WIN_WIDTH;		
	h = WIN_HEIGHT;

	//-------normal window resize code-----------

	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	moveCamera(x, y, z);

	//------end of normal window resize

	glutReshapeWindow(WIN_WIDTH, WIN_HEIGHT);	//when the window is resized, snap back to original size.  Effectively disables resize.
	//will implement a more elegant way to disable resize if I find one.
}


void drawSphere() 
{
	float colx;
	float max = 2 * aniPI;
	// Draw Body		
	if (anix <= max){
		anix += 0.01;
		colx = sin(anix);
	}
	else{
		anix = 0;
		colx = sin(anix);
	}
	glPushMatrix();
	glTranslatef(0, 0.3 * colx + 0.3 , 0);
	glColor3f(1 * colx, 0.3, 0.3);
	glTranslatef(spherePosX,spherePosY,spherePosZ);
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	GLfloat potamb[3] = {0.8,0.8,0.8},
			potdfs[3] = {0.9,0.9,0.9},
			potspc[3] = {1.0,1.0,1.0};
	GLfloat surfEmissionColour[4] = {0.2 * (zen/100.0), 0.2 * (zen/100.0), 0.2 * (zen/100.0), 1.0};		//sphere gets a little darker as you lose zen

	//glMaterialfv (GL_FRONT, GL_EMISSION, surfEmissionColour);
    glMaterialfv (GL_FRONT, GL_AMBIENT  , potamb);
    glMaterialfv (GL_FRONT, GL_DIFFUSE  , potdfs);
	glMaterialfv (GL_FRONT, GL_SPECULAR , potspc);
    glMateriali  (GL_FRONT, GL_SHININESS, 25);

	glutSolidSphere(SPHERE_RAD, 20, 20);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
	
	//set material properties back to default values
	GLfloat dullEmission[4] = {0, 0, 0, 1};
	glMaterialfv (GL_FRONT, GL_EMISSION, dullEmission);
	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

}

void initScene() {
	mazeObj->changeLevel(currentlevel);	//load first level
	glEnable(GL_DEPTH_TEST);

	glEnable( GL_TEXTURE_2D );

	//enable backface culling (i.e. if the camera goes into a wall, we'll see through the wall)
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	skyboxTexture = LoadTexture( "textures/skybox.raw", 800, 600);
	glBindTexture( GL_TEXTURE_2D, skyboxTexture);
	grassTexture = LoadTexture( "textures/grass.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, grassTexture);
	redTexture = LoadTexture( "textures/building.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, redTexture);
	brickTexture = LoadTexture( "textures/brick.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, brickTexture);
	roofTexture = LoadTexture( "textures/roof.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, roofTexture);
	treeTexture = LoadTexture( "textures/trees.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, treeTexture);
	sphereTexture = LoadTexture( "textures/sphere.raw", 300, 300);
	glBindTexture( GL_TEXTURE_2D, sphereTexture);
	introScreen = LoadTexture( "textures/titlescreen.raw", 640, 360);
	glBindTexture( GL_TEXTURE_2D, introScreen );
	
	glDisable( GL_TEXTURE_2D );
	//Scene Lighting:
	glEnable(GL_LIGHTING);		//comment out this line to turn off lighting effects
	glEnable(GL_COLOR_MATERIAL); // Keep original colours of objects
	//glShadeModel(GL_SMOOTH);
	GLfloat light1PosType [] = {2.0, 0.0, 3.0, 1.0};
	GLfloat whiteColor[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat blackColor[] = {0.0, 0.0, 0.0, 0.0};
	glEnable(GL_LIGHT0);

	//glEnable(GL_LIGHT1);
	GLfloat lightPos[] = {spherePosX, spherePosY + SPHERE_RAD/2.0, spherePosZ, 1.0f};
	GLfloat sphGlow[] = {0.3, 0.6, 0.5, 1.0};
	glLightfv (GL_LIGHT1, GL_DIFFUSE, sphGlow);
	glLightfv (GL_LIGHT1, GL_POSITION, lightPos);

	glEnable(GL_LIGHT_MODEL_AMBIENT);
	GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

}

void drawGrid() {		//draw gridlines of map cell.  Used for debugging
	const float heightAboveGround = 0.3;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, heightAboveGround, 0);
		glVertex3f(40.0, heightAboveGround, 0);
		glVertex3f(40.0, heightAboveGround, 40.0);
		glVertex3f(0, heightAboveGround, 40.0);
	glEnd();
	glPopMatrix();
}

void drawMaze(void) { //draw walls, obstacles, other map features	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR); 
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR); 

	for(int i = -(MAP_SIDE/2); i < (MAP_SIDE/2); i++) {
			for(int j=-(MAP_SIDE/2); j < (MAP_SIDE/2); j++) {
				glPushMatrix();
				glTranslatef(i * CELL_SIDE, 0, j * CELL_SIDE);
				if (dev_mode) {
					drawGrid();		//draw gridlines if devmode is enabled
				}
				switch (mazeObj->checkMaze((i + MAP_SIDE/2), (j + MAP_SIDE/2))) {
					case 1:
						glBindTexture( GL_TEXTURE_2D, brickTexture);
						drawing->drawXWall();
						break;
					case 2:
						glBindTexture( GL_TEXTURE_2D, brickTexture);
						drawing->drawZWall();
						break;
					case 3:
						drawing->drawTemples(roofTexture, redTexture);
						break;
					case 4:
						drawing->drawWiseMen();
						break;
					case 5:
						glBindTexture( GL_TEXTURE_2D, treeTexture);
						drawing->drawTree();
						break;
				}
				glPopMatrix();
			}
	}
}

void moveMeFlat(float i) {		//moving forward/back by i units
	i = -i;		//forward = +'ve, backward = -'ve
	//new sphere coords:
	float newPosX = spherePosX + i * cos(angle);	//sphere moves in a straight line in the direction of the camera angle
	float newPosY = spherePosY;
	float newPosZ = spherePosZ + i * sin(angle);

	if (playAgainMode == 0) {
		if (collisions->checkCollision(newPosX, newPosY, newPosZ, SPHERE_RAD)) {	//check if there are obstacles in the intended location
			if (i > COLLISION_SPACING){
				moveMeFlat(i - COLLISION_SPACING);	//attempt to move forward in a shorter jump.  This way you don't get stopped at a distance from the obstacle
				if (mazeObj->checkMaze(collisions->getGridPositionX(spherePosX), collisions->getGridPositionZ(spherePosZ)) == 4) {
					// This checks if you've collided with a Wise Man.  If you have, he should ask his question.
					question_mode = true;
				}
			}
		if (i < -COLLISION_SPACING){
			moveMeFlat(i + COLLISION_SPACING);	//to account for backwards movement
			if (mazeObj->checkMaze(collisions->getGridPositionX(spherePosX), collisions->getGridPositionZ(spherePosZ)) == 4) {
				// This checks if you've collided with a Wise Man.  If you have, he should ask his question.
				// This currently doesn't work.  Advice appreciated!
				question_mode = true;
				}
		}
		} else {
			//move sphere
			spherePosX = newPosX;
			spherePosY = newPosY;
			spherePosZ = newPosZ;

			GLfloat lightPos[] = {spherePosX, spherePosY + SPHERE_RAD/2.0, spherePosZ, 1.0f};
			glLightfv (GL_LIGHT1, GL_POSITION, lightPos);

			//new camera coords:
			z = z + i * sin(angle);
			x = x + i * cos(angle);

			moveCamera(x, y, z);
			if (i != 0.0) moveCount++;
		}
	}
}

void orientMe(float ang) {		//turning
	angle -= ang;

	x = spherePosX + CAM_DIST * cos(angle);
	z = spherePosZ + CAM_DIST * sin(angle);

	moveCamera(x, y, z);
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!intro_mode) {
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, grassTexture);
		drawing->drawGround();
		
		glBindTexture( GL_TEXTURE_2D, skyboxTexture );
		drawing->drawSkybox();
		
		//deal with sphere movement (maybe this should be done elsewhere?):
		if (sphForwardVel != 0.0f) moveMeFlat(sphForwardVel);	//move sphere and camera
		if (sphRotVel != 0.0f) orientMe(sphRotVel);		//face the appropriate direction
		if (!dirBtnDown) {			//if the user is not pressing a directional button, slow down
			if (sphForwardVel > DECEL) {
				sphForwardVel -= DECEL;
			} else if (sphForwardVel < -DECEL) {
				sphForwardVel += DECEL;
			} else {
				sphForwardVel = 0.0;
			}
		}
		if (!rotBtnDown) {			//if the user is not pressing a turn button, slow rotation
			if (sphRotVel > ROT_DECEL) {
				sphRotVel -= ROT_DECEL;
			} else if (sphRotVel < -ROT_DECEL) {
				sphRotVel += ROT_DECEL;
			} else {
				sphRotVel = 0.0;
			}
		}
		
		glBindTexture( GL_TEXTURE_2D, sphereTexture);
		drawSphere();
		glDisable( GL_TEXTURE_2D );

		drawMaze();

		if (moveCount < 30) hud->drawIntroText();		//display the intro/instructions until the user moves a little
		if (moveCount - moveCountMsgMark < 80) {		//display save/load status messages for a set amount of moves
			switch (showSaveLoadMsg) {
				case 1:
					//save success
					hud->printSaveLoadFeedback(true, true);
					break;
				case 2:
					//save fail
					hud->printSaveLoadFeedback(true, false);
					break;
				case 3:
					//load success
					hud->printSaveLoadFeedback(false, true);
					break;
				case 4:
					//load fail
					hud->printSaveLoadFeedback(false, false);
					break;
				default:
					break;
			}
		} else {
			showSaveLoadMsg = 0;
		}
		if (moveCount - moveCountQstnMark < 20) {
			switch (showQuestionMsg) {
				case 1:
					hud->printQuestionFeedback(true);	//show correct answer feedback
					break;
				case 2:
					hud->printQuestionFeedback(false);	//show wrong answer feedback
					break;
				default:
					break;
			}
		} else {
			showQuestionMsg = 0;
		}

		hud->drawHUD();		//HUD must be drawn last
		
		if (question_mode) hud->drawQuestion(challenges.getCurrentChallenge(), challenges.getFirstChoice(), challenges.getSecondChoice(), challenges.getThirdChoice());

		if (playAgainMode==1) hud->printPlayAgainMsg(true);
		else if (playAgainMode==2) hud->printPlayAgainMsg(false);
	} else {
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, introScreen);
		hud->drawIntroScreen();
		glDisable( GL_TEXTURE_2D );
	}

	glutSwapBuffers();
}

void playAgain() {		//restart game from beginning
	//basically just set all the variables back to initial values.
	angle=0.0;
	x=0.0f, y=1.75f, z=5.0f;
	spherePosX = -120.0;
	spherePosY = 0.0 + SPHERE_RAD;
	spherePosZ = 0;
	sphForwardVel = 0.0f;
	sphRotVel = 0.0f;
	anix = 0;
	aniPI = 3.1415926535897932384626433832795;
	dirBtnDown = false;
	rotBtnDown = false;
	sphereRotX = 0.0;
	sphereRotY = 0.0;
	sphereRotZ = 0.0;
	sphereRotAng = 0.0;
	dev_mode = false;
	playAgainMode = 0;
	moveCount = 0;
	showSaveLoadMsg = 0;
	moveCountMsgMark = 0;
	zen = 100;
	level = 1;
	currentlevel = 1;
	orientMe(-PI/2);
	question_mode = false;
	mazeObj->changeLevel(currentlevel);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void win() {	//player has won game
	question_mode = false;
	playAgainMode = 1;	//display "You win, play again?" message
	GLfloat globalAmbient[] = {0.7, 1.0, 0.7, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void lose() {	//player has lost
	question_mode = false;
	playAgainMode = 2;	//display "You lose, play again?" message
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	GLfloat globalAmbient[] = {0.8, 0.5, 0.5, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (intro_mode) {
		if (key == '1') {
			difficulty_mod = 1;
			intro_mode = false;
		} else if (key == '2') {
			difficulty_mod = 2;
			intro_mode = false;
		} else if (key == '3') {
			difficulty_mod = 5;
			intro_mode = false;
		}
	} else if (playAgainMode != 0) {	//if the user is being asked to play again, handle Y/N choice
		if ((key == 'Y') || (key == 'y')) {
			playAgain();		//play again
		} else if ((key == 'N') || (key == 'n')) {
			key = 27;			//set key to Esc -> quit game
		}
	}
	if ((key == 27) || (key == 'q') || (key == 'Q')) {
		exit(0);
	} else if ((key == 'j') || (key == 'J')) {		//enable dev mode (draws gridlines on the map)
		dev_mode = !dev_mode;
	} else if ((key == 'r') || (key == 'R')) {		//restart game
		playAgain();
	}

}

void levelchange(int newLevel)
{
	angle=0.0;
	x=0.0f, y=1.75f, z=5.0f;
	sphForwardVel = 0.0f;
	sphRotVel = 0.0f;
	anix = 0;
	aniPI = 3.1415926535897932384626433832795;
	dirBtnDown = false;
	rotBtnDown = false;
	sphereRotX = 0.0;
	sphereRotY = 0.0;
	sphereRotZ = 0.0;
	sphereRotAng = 0.0;
	orientMe(-PI/2);

	spherePosX = -180;
	spherePosY = 0.0 + SPHERE_RAD;
	spherePosZ = -180;

	if (newLevel <= 5) mazeObj->changeLevel(newLevel);
	else {
		mazeObj->changeLevel(6);
		orientMe(-PI/4);	//gives a better view of the field
	}
	orientMe(-PI/2);
}

void answerquestion (int answer){
	
	bool result;
	
	question_mode = false;
	result = challenges.answerChallenge(answer);
	
	if (result){
		level++;
		showQuestionMsg = 1;				//show correct answer feedback
		moveCountQstnMark = moveCount;
		if (level >= WIN_LEVEL) win();	//game is won when plane of conciousness gets to a certain level
		levelchange(level);				//ascend to the next plane! (next level)
	} else {
		showQuestionMsg = 2;
		moveCountQstnMark = moveCount;
		if (zen - difficulty_mod * ZEN_PENALTY > 0) zen -= difficulty_mod * ZEN_PENALTY;
		else {
			zen = 0;
			lose();
		}
	}
	currentlevel++;
	//levelchange(currentlevel);
}

void inputKey(int key, int x, int y) {
	if (playAgainMode == 0) {	//don't let the player move if they are in a win or loss state
		switch (key) {
			case GLUT_KEY_LEFT :
				if(!question_mode){
				sphRotVel = TURN_ANGLE;
				rotBtnDown = true;}
				break;
			case GLUT_KEY_RIGHT : 
				if(!question_mode){
				sphRotVel = -TURN_ANGLE;
				rotBtnDown = true;}
				break;
			case GLUT_KEY_UP :
				if(!question_mode){
				sphForwardVel = MOVE_SIZE;		//set forward velocity
				dirBtnDown = true;}			//up or down is pressed
				break;
			case GLUT_KEY_DOWN : 
				if(!question_mode){
				sphForwardVel = -MOVE_SIZE;
				dirBtnDown = true;}
				break;
			//answering questions:
			case GLUT_KEY_F1 :
				if (question_mode) answerquestion(0);
				break;
			case GLUT_KEY_F2 :
				if (question_mode) answerquestion(1);
				break;
			case GLUT_KEY_F3 :
				if (question_mode) answerquestion(2);
				break;
			//cheats:
			//skip levels
			case GLUT_KEY_F4 :
				currentlevel++;
				level++;
				levelchange(currentlevel);
				break;
			//auto-win
			case GLUT_KEY_F5:
				win();
				break;
			//auto-lose
			case GLUT_KEY_F6:
				lose();
				break;
		}
	}
}

bool saveGame(int slot) {		//saves game in the specified slot. Returns true if successful, false if failed.
	mkdir("saves");		//create a 'saves' directory, if it's not already there
	stringstream out;
	out << slot;
	string fileName = "saves\\spheresave" + out.str() + ".sph";
	ofstream saveSlot(fileName.c_str()); // Open the specified file for writing

	if (saveSlot.is_open()){ // If the file opened successfully, we can proceed.
		//basically just write all relevant variables to the save file.
		saveSlot << angle << " ";
		saveSlot << ratio << " ";
		saveSlot << x << " ";
		saveSlot << y << " ";
		saveSlot << z << " ";
		saveSlot << spherePosX << " ";
		saveSlot << spherePosY << " ";
		saveSlot << spherePosZ << " ";
		saveSlot << sphForwardVel << " ";
		saveSlot << sphRotVel << " ";
		saveSlot << anix << " ";
		saveSlot << aniPI << " ";
		saveSlot << dirBtnDown << " ";
		saveSlot << rotBtnDown << " ";
		saveSlot << sphereRotX << " ";
		saveSlot << sphereRotY << " ";
		saveSlot << sphereRotZ << " ";
		saveSlot << sphereRotAng << " ";
		saveSlot << dev_mode << " ";
		saveSlot << playAgainMode << " ";
		saveSlot << moveCount << " ";
		saveSlot << showSaveLoadMsg << " ";
		saveSlot << moveCountMsgMark << " ";
		saveSlot << zen << " ";
		saveSlot << level << " ";
		saveSlot << currentlevel << " ";
		saveSlot << challenges.getChallengeNum() << " ";
		saveSlot << difficulty_mod << " ";

		saveSlot.close(); // We're done with the file, so close it now.
		return true;
	} else {
		return false;	//save not successful
	}
}

bool loadGame(int slot) {		//loads game from the specified slot. Returns true if successful, false if failed.
	stringstream out;
	out << slot;
	string fileName = "saves\\spheresave" + out.str() + ".sph";
	ifstream loadSlot(fileName.c_str()); // Open the specified file

	if (loadSlot.is_open()){ // If the file opened successfully, we can proceed.
		//basically just read all relevant variables from the save file.
		loadSlot >> angle;
		loadSlot >> ratio;
		loadSlot >> x;
		loadSlot >> y;
		loadSlot >> z;
		loadSlot >> spherePosX;
		loadSlot >> spherePosY;
		loadSlot >> spherePosZ;
		loadSlot >> sphForwardVel;
		loadSlot >> sphRotVel;
		loadSlot >> anix;
		loadSlot >> aniPI;
		loadSlot >> dirBtnDown;
		loadSlot >> rotBtnDown;
		loadSlot >> sphereRotX;
		loadSlot >> sphereRotY;
		loadSlot >> sphereRotZ;
		loadSlot >> sphereRotAng;
		loadSlot >> dev_mode;
		loadSlot >> playAgainMode;
		loadSlot >> moveCount;
		loadSlot >> showSaveLoadMsg;
		loadSlot >> moveCountMsgMark;
		loadSlot >> zen;
		loadSlot >> level;
		loadSlot >> currentlevel;
		int chalNum;
		loadSlot >> chalNum;
		challenges.setChallengeNum(chalNum);
		loadSlot >> difficulty_mod;
		
		loadSlot.close(); // We're done with the file, so close it now.
		mazeObj->changeLevel(level);
		orientMe(sphRotVel);
		return true;
	} else {
		return false;		//couldn't open file
	}
}

void menu(GLint selection) {		//define right click menu
	switch (selection) {
		case 1:
		case 2:
		case 3:
			//save game to slot <selection>
			if (saveGame(selection)) {
				//show success msg
				showSaveLoadMsg = 1;
			} else {
				//show fail msg
				showSaveLoadMsg = 2;
			}
			break;
		case 4:
		case 5:
		case 6:
			//load game from slot <selection-3>
			if (loadGame(selection-3)) {
				//show success message
				showSaveLoadMsg = 3;
			} else {
				//show failure message
				showSaveLoadMsg = 4;
			}
			break;
	}
	moveCountMsgMark = moveCount;
	glutPostRedisplay();
}

void keyUp (int key, int x, int y) {		//called when a key is released
		switch (key) {
			case GLUT_KEY_LEFT :
				rotBtnDown = false;
				break;
			case GLUT_KEY_RIGHT : 
				rotBtnDown = false;
				break;
			case GLUT_KEY_UP :
				dirBtnDown = false;			//up or down is not pressed
				break;
			case GLUT_KEY_DOWN : 
				dirBtnDown = false;			//up or down is not pressed
				break;
		}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("SphereQuest");
	
	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);
	glutSpecialUpFunc(keyUp);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutCreateMenu(menu);		//create right-click menu
		glutAddMenuEntry("Save Slot 1", 1);
		glutAddMenuEntry("Save Slot 2", 2);
		glutAddMenuEntry("Save Slot 3", 3);
		glutAddMenuEntry("Load Slot 1", 4);
		glutAddMenuEntry("Load Slot 2", 5);
		glutAddMenuEntry("Load Slot 3", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	orientMe(-PI/2.0);
	moveCamera(x, y, z);

	glutMainLoop();

	FreeTexture(skyboxTexture);
	FreeTexture(grassTexture);
	FreeTexture(sphereTexture);
	FreeTexture(brickTexture);
	FreeTexture(roofTexture);
	FreeTexture(redTexture);
	FreeTexture(treeTexture);

	return(0);
}
