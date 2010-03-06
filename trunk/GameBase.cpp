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

#include "hud.h"
#include "collision.h"
#include "maze.h"

#define CAM_DIST 20.0		//distance camera keeps from sphere
#define PI 3.14159265358979323846
#define TURN_ANGLE PI/32.0f		//angle to turn camera on keypress
#define MOVE_SIZE 5				//distance to move sphere with each keypress
#define SPHERE_RAD 2.0f			//sphere radius

//window dimensions:
#define WIN_WIDTH 640
#define WIN_HEIGHT 360

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;
//static float lx=0.0f,ly=0.0f, lz=-1.0f;
static int zen = 100;
static int level = 1;

//sphere position:
GLfloat spherePosX = 0.0;
GLfloat spherePosY = 0.0 + SPHERE_RAD;
GLfloat spherePosZ = -15.0;
//sphere rotation:
int sphereRotX = 0.0;
int sphereRotY = 0.0;
int sphereRotZ = 0.0;
float sphereRotAng = 0.0;

bool dev_mode = false;

GLuint grassTexture;
GLuint skyboxTexture;

static HUD *hud = new HUD();  // Create a HUD object
static maze *mazeObj = new maze();		//create maze!
static collision *collisions = new collision(mazeObj);		//create a new collision-checker!

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
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(spherePosX,spherePosY,spherePosZ);
	glRotatef(sphereRotAng, sphereRotX, sphereRotY, sphereRotZ);
	glutSolidSphere(SPHERE_RAD, 20, 20);
	glPopMatrix();
}

void drawTemples() {
	glPushMatrix();
	
	//Draw Roof
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,32);
	glColor3f(0, 0, 0);
	glutSolidCone(40, 30, 20, 20);
	glPopMatrix();

	glTranslatef(0, 20, 0);
	glColor3f(0.5, 0, 0);
	glutSolidCube(40);

		//Draw Roof
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,45);
	glColor3f(0, 0, 0);
	glutSolidCone(30, 20, 20, 20);
	glPopMatrix();

	glTranslatef(0, 35, 0);
	glColor3f(0.5, 0, 0);
	glutSolidCube(30);

			//Draw Roof
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,35);
	glColor3f(0, 0, 0);
	glutSolidCone(20, 10, 20, 20);
	glPopMatrix();

	glTranslatef(0, 25, 0);
	glColor3f(0.5, 0, 0);
	glutSolidCube(20);
	glPopMatrix();
}

void drawXWall() {

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(-10, 10, 0);
	glutSolidCube(20);
	glTranslatef(20, 0, 0);
	glutSolidCube(20);
	glPopMatrix();
}

void drawZWall() {
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0, 10, -10);
	glutSolidCube(20);
	glTranslatef(0, 0, 20);
	glutSolidCube(20);
	glPopMatrix();
}
void drawTree() {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 0.4, 0);
	glTranslatef(0, 0, 0);
	glutSolidCone(3, 5, 10, 10);
	glTranslatef(0, 0, 2.5);
	glutSolidCone(3, 5, 10, 10);
	glTranslatef(0, 0, 2.5);
	glutSolidCone(3, 5, 10, 10);
	glPopMatrix();
}

void drawWiseMen() {


	// Draw Body	
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.5f);
	glTranslatef(0.0f ,0.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.6f,2.0f,20,20);
	glPopMatrix();

	//Draw Belt
	glPushMatrix();
	glColor3f(0.5f, 0.25f, 0.0f);
	glTranslatef(0.0f ,0.7f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(0.19f, 0.3f,10,10);
	glPopMatrix();

	// Draw Head
	glColor3f(1.0f, 0.75f, 0.6f);
	glTranslatef(0.0f, 1.8f, 0.0f);
	glutSolidSphere(0.35f,20,20);
	
	//Draw Beard
	glPushMatrix();
	glColor3f(0.5f,0.5f,0.5f);
	glTranslatef(0.0f, -0.25f, 0.05f);
	glRotatef(50.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.2f,0.8f,20,20);
	glPopMatrix();

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.13f, 0.0f, 0.30f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.26f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

	//Draw Eyebrows
	glPushMatrix();
	glColor3f(0.5,0.5,0.5);
	glTranslatef(-0.26f, 0.1f, 0.35f);
	glRotatef(90.0, 0.0f, 1.0f, -0.2f);
	glutSolidCone(0.04,0.2,5,5);
	glTranslatef(0.0f, 0.1f, 0.52f);
	glRotatef(-180.0, 0.0f, 1.0f, -0.2f);
	glutSolidCone(0.04,0.2,5,5);
	glPopMatrix();

	// Draw Hat
	glPushMatrix();
	glTranslatef(0.0f, 0.1f, 0.0f);
	glColor3f(1.0f, 0.8f , 0.2f);
	glRotatef(-90.0f ,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.9f,0.35f,10,2);
	glPopMatrix();

}

void initScene() {

	glEnable(GL_DEPTH_TEST);

	glEnable( GL_TEXTURE_2D );

	skyboxTexture = LoadTexture( "textures/skybox.raw", 800, 600);
	glBindTexture( GL_TEXTURE_2D, skyboxTexture);
	grassTexture = LoadTexture( "textures/grass.raw", 1024, 1024 );
	glBindTexture( GL_TEXTURE_2D, grassTexture);
	
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
	glLightfv (GL_LIGHT1, GL_AMBIENT, whiteColor);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, whiteColor);
	glLightfv (GL_LIGHT1, GL_SPECULAR, whiteColor);
	glEnable(GL_LIGHT_MODEL_AMBIENT);
	GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

}

void drawGround(){
	// Draw ground
	glBegin(GL_QUADS);
	//glColor3f(0.2f, 0.7f, 0.2f);
	//glColor3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-200.0f, 0.0f, -200.0f);
	//glColor3f(0, 1, 0);
	glTexCoord2f(50, 0);
	glVertex3f(-200.0f, 0.0f,  200.0f);
	//glColor3f(0, 0, 1);
	glTexCoord2f(50, 50);
	glVertex3f( 200.0f, 0.0f,  200.0f);
	//glColor3f(1, 1, 1);
	glTexCoord2f(0, 50);
	glVertex3f( 200.0f, 0.0f, -200.0f);
	glEnd();
}

//Sky is pretty
void drawSkybox(){
	glColor3f(0.8, 0.9, 1.0);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidSphere(300, 10, 10);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

//Couldn't resist. May put this in skybox texture later
void drawMtFuji() {
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 400, 0);
	glutSolidCone(200, 200, 20, 20);
	glPopMatrix();
}

void drawGrid() {		//show gridlines.  Used for debugging
	//CARRY ON MY FARBOD SON
	//THERE'LL BE PEACE WHEN YOU ARE DONE
}

void drawMaze(void) {		//draw walls, obstacles, other level features
	for(int i = -(MAP_SIDE/2); i < (MAP_SIDE/2); i++) {
			for(int j=-(MAP_SIDE/2); j < (MAP_SIDE/2); j++) {
				glPushMatrix();
				glTranslatef(i * CELL_SIDE, 0, j * CELL_SIDE);
				switch (mazeObj->checkMaze((i + MAP_SIDE/2), (j + MAP_SIDE/2))) {
					case 1:
						drawXWall();
						break;
					case 2:
						drawZWall();
						break;
					case 3:
						drawTemples();
						break;
					case 4:
						drawWiseMen();
						break;
					case 5:
						drawTree();
						break;
				}
				glPopMatrix();
			}
	}
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, grassTexture);
	drawGround();
	//FreeTexture( grassTexture );
	
	glBindTexture( GL_TEXTURE_2D, skyboxTexture );

	drawSkybox();
	
	//FreeTexture( skyboxTexture );
	glDisable( GL_TEXTURE_2D );
	
	drawSphere();
	if (dev_mode) drawGrid();

	drawMaze();

	//drawMtFuji();	

	hud->drawHUD();		//HUD must be drawn last
	glutSwapBuffers();
}

void orientMe(float ang) {		//turning
	angle -= ang;

	x = spherePosX + CAM_DIST * cos(angle);
	z = spherePosZ + CAM_DIST * sin(angle);

	moveCamera(x, y, z);
}

void moveMeFlat(int i) {		//moving forward/back by i units
	float oldX = spherePosX;
	float oldZ = spherePosZ;
	//new sphere coords:
	spherePosX = spherePosX + i * cos(angle);	//sphere moves in a straight line in the direction of the camera angle
	spherePosZ = spherePosZ + i * sin(angle);
	if (collisions->checkCollision(spherePosX, spherePosY, spherePosZ, SPHERE_RAD)) {	//check if there are obstacles in the intended location
		spherePosX = oldX;	//reset sphere coords
		spherePosZ = oldZ;
	} else {		
		//new camera coords:
		z = z + i * sin(angle);
		x = x + i * cos(angle);

		moveCamera(x, y, z);
	}
}

void processNormalKeys(unsigned char key, int x, int y) {

	if ((key == 27) || (key = 'q') || (key = 'Q'))
		exit(0);
	else if (key == 'j')	//enable debug mode
		dev_mode = true;
}


void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
			orientMe(TURN_ANGLE);
			//sphereRotX = (sphereRotX - 5) % 360;
			break;
		case GLUT_KEY_RIGHT : 
			orientMe(-TURN_ANGLE);
			//sphereRotX = (sphereRotX + 5) % 360;
			break;
		case GLUT_KEY_UP :
			moveMeFlat(-MOVE_SIZE);
			//sphereRotY = (sphereRotY + 5) % 360;
			break;
		case GLUT_KEY_DOWN : 
			moveMeFlat(MOVE_SIZE);
			//sphereRotY = (sphereRotY - 5) % 360;
			break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("SPHEREQUEST");
	
	//Get the maze set for the level
//	mazeObj->generateMaze();
	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	//glutDisplayFunc(drawHUD);

	glutReshapeFunc(changeSize);

	orientMe(-PI/2.0);
	moveCamera(x, y, z);

	glutMainLoop();

	return(0);
}
