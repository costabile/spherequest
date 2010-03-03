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
int maze[6][6]= {
	{3, 2, 0, 3, 2, 3},
	{1, 0, 0, 1, 5, 1},
	{1, 0, 2, 3, 0, 1},
	{1, 0, 0, 0, 0, 3},
	{1, 5, 4, 1, 0, 1},
	{3, 2, 2, 3, 4, 3}
	};
float sphereRotAng = 0.0;

GLuint grassTexture;
GLuint skyboxTexture;

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

void generateMaze()
{
	//Empty Maze Matrix
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			maze[i][j] = 0;
		}
	} 
	//Define the following maze (1 = wall, 2 = Wise Man!)
	// 1 1 0 1 1 1
	// 1 0 0 1 0 1
	// 1 0 1 0 0 1
	// 1 0 0 0 1 1
	// 1 0 1 0 0 1
	// 1 1 1 1 2 1

	//The following is an EXTREMELY brute force way of defining a map.
	//The maps will later be read from a file!
	for (int i = 0; i < 6; i++)
	{
		maze[i][0] = 1;
		maze[i][5] = 1;
	}

	for (int j = 0; j < 6; j++)
	{
		maze[0][j] = 1;
		maze[5][j] = 1;
	}

	maze[0][2] = 0;
	maze[5][4] = 2;
	maze[1][3] = 1;
	maze[2][2] = 1;
	maze[3][4] = 1;
	maze[4][2] = 1;
}

//A quick call to see what the value of maze is
int checkMaze(int row, int column)
{
	return maze[row][column];

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
	//snowman_display_list = createDL();
	
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
	glVertex3f(-200.0f, 0.0f, -200.0f);
	//glColor3f(0, 1, 0);
	glVertex3f(-200.0f, 0.0f,  200.0f);
	//glColor3f(0, 0, 1);
	glVertex3f( 200.0f, 0.0f,  200.0f);
	//glColor3f(1, 1, 1);
	glVertex3f( 200.0f, 0.0f, -200.0f);
	glEnd();
}

//Sky is pretty
void drawSkybox(){
	glColor3f(0.8, 0.9, 1.0);
	glutSolidSphere(400, 10, 10);
}

//Couldn't resist. May put this in skybox texture later
void drawMtFuji(){
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 400, 0);
	glutSolidCone(200, 200, 20, 20);
	glPopMatrix();
}

void printText(float x, float y, char *string, float r, float g, float b)		//renders string on the screen at coords (x, y) in color (r, g, b)
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

void drawHUD() {		//draws a 2D overlay

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

}

void drawMaze(void) {		//draw walls, obstacles, other level features
	for(int i = -3; i < 3; i++) {
			for(int j=-3; j < 3; j++) {
				glPushMatrix();
				glTranslatef(i * 40.0, 0, j * 40.0);
				switch (checkMaze((i + 3), (j + 3))) {
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
	
	grassTexture = LoadTexture( "textures/test.raw", 256, 256 );
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
	
	glBindTexture( GL_TEXTURE_2D, grassTexture);

	drawGround();

	FreeTexture( grassTexture );

	drawSphere();
	drawSkybox();
	drawMtFuji();	
	drawMaze();
	drawHUD();		//HUD must be drawn last

	glutSwapBuffers();
}

bool checkCollision(int movX, int movY, int movZ) {		//checks whether moving to movX, movY, movZ would be a collision
	bool isCollision = false;

	//todo

	return isCollision;	
}

void orientMe(float ang) {		//turning
	angle -= ang;

	x = spherePosX + CAM_DIST * cos(angle);
	z = spherePosZ + CAM_DIST * sin(angle);

	moveCamera(x, y, z);
}

void moveMeFlat(int i) {		//moving forward/back
	//new camera coords:
	z = z + i * sin(angle);
	x = x + i * cos(angle);
	//new sphere coords:
	spherePosX = spherePosX + i * cos(angle);	//sphere moves in a straight line in the direction of the camera angle
	spherePosZ = spherePosZ + i * sin(angle);

	moveCamera(x, y, z);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if ((key == 27) || (key = 'q') || (key = 'Q'))
		exit(0);
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
//	generateMaze();
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