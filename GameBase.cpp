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

#include "challenge.h"
#include "hud.h"
#include "player.h"
#include "collision.h"
#include "maze.h"
#include "draw.h"

#define PI 3.14159265358979323846
#define SPHERE_RAD 2.0f				//sphere radius
#define CAM_DIST 20.0				//distance camera keeps from sphere
#define TURN_ANGLE PI/64.0f			//angle to turn camera on keypress
#define MOVE_SIZE 0.5f				//base sphere movement distance
#define ROT_ANG PI/64.0f			//angle to rotate sphere by each time it moves
#define DECEL 0.1f					//the rate at which the sphere's movement slows when the user is not pressing forward or back
#define ROT_DECEL TURN_ANGLE/2.5		//rate at which rotation slows when left or right are not pressed
#define COLLISION_SPACING 0.2f		//the max distance the sphere might be from an obstacle when it stops moving.  Smaller = more precise collisions.

//window dimensions:
#define WIN_WIDTH 640
#define WIN_HEIGHT 360

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;		//camera coords
//static float lx=0.0f,ly=0.0f, lz=-1.0f;
//static int zen = 100;
//static int level = 1;

//sphere position (initial = -60, 0+radius, -9.5):
GLfloat spherePosX = -60.0;
GLfloat spherePosY = 0.0 + SPHERE_RAD;
GLfloat spherePosZ = -9.5;
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
int moveCount = 0;			//counts the number of sphere movements

GLuint grassTexture;
GLuint skyboxTexture;
GLuint sphereTexture;
GLuint brickTexture;
GLuint roofTexture;
GLuint redTexture;
GLuint treeTexture;

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
	glTranslatef(0, 0.2 * colx + 0.2 , 0);
	//glRotatef(0, 0, sphereRotAng, 1);
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

	//glRotatef (-90, 0, 0, 1);

	glPopMatrix();
	
	//set material properties back to default values
	GLfloat dullEmission[4] = {0, 0, 0, 1};
	glMaterialfv (GL_FRONT, GL_EMISSION, dullEmission);
	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

}

void initScene() {
	mazeObj->readMaze("mazes/level1.txt");
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
	
	glDisable( GL_TEXTURE_2D );
	//Scene Lighting:
	glEnable(GL_LIGHTING);		//comment out this line to turn off lighting effects
	glEnable(GL_COLOR_MATERIAL); // Keep original colours of objects
	//glShadeModel(GL_SMOOTH);
	GLfloat light1PosType [] = {2.0, 0.0, 3.0, 1.0};
	GLfloat whiteColor[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat blackColor[] = {0.0, 0.0, 0.0, 0.0};
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.5);
	glEnable(GL_LIGHT0);

	//glEnable(GL_LIGHT1);
	GLfloat lightPos[] = {spherePosX, spherePosY + SPHERE_RAD/2.0, spherePosZ, 1.0f};
	GLfloat sphGlow[] = {0.3, 0.6, 0.5, 1.0};
	//glLightfv (GL_LIGHT1, GL_AMBIENT, whiteColor);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, sphGlow);
	//glLightfv (GL_LIGHT1, GL_SPECULAR, sphGlow);
	glLightfv (GL_LIGHT1, GL_POSITION, lightPos);

	glEnable(GL_LIGHT_MODEL_AMBIENT);
	GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};
	//GLfloat globalAmbient[] = {0.1, 0.1, 0.1, 1.0};
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
	if (collisions->checkCollision(newPosX, newPosY, newPosZ, SPHERE_RAD)) {	//check if there are obstacles in the intended location
		if (i > COLLISION_SPACING)
			moveMeFlat(i - COLLISION_SPACING);	//attempt to move forward in a shorter jump.  This way you don't get stopped at a distance from the obstacle
		if (i < -COLLISION_SPACING)
			moveMeFlat(i + COLLISION_SPACING);	//to account for backwards movement
	} else {
		//move sphere
		spherePosX = newPosX;
		spherePosY = newPosY;
		spherePosZ = newPosZ;

		GLfloat lightPos[] = {spherePosX, spherePosY + SPHERE_RAD/2.0, spherePosZ, 1.0f};
		glLightfv (GL_LIGHT1, GL_POSITION, lightPos);

		//rotate sphere (make it look like it's rolling):
		if (i > 0.0) sphereRotAng += ROT_ANG;
		else if (i < 0.0) sphereRotAng -= ROT_ANG;

		//new camera coords:
		z = z + i * sin(angle);
		x = x + i * cos(angle);

		moveCamera(x, y, z);
		if (i != 0.0) moveCount++;
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

	glEnable( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, grassTexture);
	drawing->drawGround();
	//FreeTexture( grassTexture );
	
	glBindTexture( GL_TEXTURE_2D, skyboxTexture );
	drawing->drawSkybox();
	
	//FreeTexture( skyboxTexture );
	
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
	if (!rotBtnDown) {			//the the user is not pressing a turn button, slow rotation
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

	//drawMtFuji();	

	if (moveCount < 30) hud->drawIntroText();		//display the intro/instructions until the user moves a little

	hud->drawHUD();		//HUD must be drawn last
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if ((key == 27) || (key == 'q') || (key == 'Q')) {
		exit(0);
	} else if (key == 'j') {		//enable dev mode (draws gridlines on the map)
		dev_mode = !dev_mode;
	}
}


void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
			sphRotVel = TURN_ANGLE;
			rotBtnDown = true;
			break;
		case GLUT_KEY_RIGHT : 
			sphRotVel = -TURN_ANGLE;
			rotBtnDown = true;
			break;
		case GLUT_KEY_UP :
			sphForwardVel = MOVE_SIZE;		//set forward velocity
			dirBtnDown = true;			//up or down is pressed
			break;
		case GLUT_KEY_DOWN : 
			sphForwardVel = -MOVE_SIZE;
			dirBtnDown = true;
			break;
	}
}

void menu(GLint selection) {		//create right-click menu
	switch (selection) {
		case 0:
			//do somethin'
			break;
		case 1:
			//do somethin' else
			break;
	}
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
	
	//Get the maze set for the level
//	mazeObj->generateMaze();
	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);
	glutSpecialUpFunc(keyUp);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutCreateMenu(menu);		//create right-click menu
		glutAddMenuEntry("Save Game", 0);
		glutAddMenuEntry("Load Game", 1);
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
