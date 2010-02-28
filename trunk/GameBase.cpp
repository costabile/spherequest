//#include <glut.h>
#include <math.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <glut.h>

#define CAM_DIST 20.0		//distance camera keeps from sphere
#define PI 3.14159265358979323846
#define TURN_ANGLE PI/32.0f		//angle to turn camera on keypress
#define MOVE_SIZE 5				//distance to move sphere with each keypress
#define SPHERE_RAD 2.0f			//sphere radius

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;
static float lx=0.0f,ly=0.0f, lz=-1.0f;
static GLint snowman_display_list;

//sphere position:
GLfloat spherePosX = 0.0;
GLfloat spherePosY = 0.0 + SPHERE_RAD;
GLfloat spherePosZ = -15.0;
//sphere rotation:
int sphereRotX = 0.0;
int sphereRotY = 0.0;
int sphereRotZ = 0.0;
int maze[6][6];
float sphereRotAng = 0.0;

void moveCamera(float xDif, float yDif, float zDif) {		//moves the camera to xDif, yDif, zDif (always looking at sphere)
	
	glLoadIdentity();
	gluLookAt(xDif, yDif, zDif, 
		//x + lx,y + ly,z + lz,		//original
		spherePosX, spherePosY, spherePosZ,		//always look at sphere
		0.0f,1.0f,0.0f);
}

void changeSize(int w, int h)
{

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
	/*glLoadIdentity();
	gluLookAt(x, y, z, 
		x + lx,y + ly,z + lz,
		0.0f,1.0f,0.0f);*/
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

}

void drawHorizontalWall() {

	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCube(39);
	glPopMatrix();
}

void drawVerticalWall() {

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

GLuint createDL() {
	GLuint snowManDL;

	// Create the id for the list
	snowManDL = glGenLists(1);

	// start list
	glNewList(snowManDL,GL_COMPILE);

	// call the function that contains the rendering commands
	drawWiseMen();

	// endList
	glEndList();

	return(snowManDL);
}

void initScene() {

	glEnable(GL_DEPTH_TEST);
	snowman_display_list = createDL();
	
	//Scene Lighting:
	//glEnable(GL_LIGHTING);		//comment out this line to turn off lighting effects
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
	glColor3f(0.2f, 0.7f, 0.2f);
	glBegin(GL_QUADS);
	glVertex3f(-200.0f, 0.0f, -200.0f);
	glVertex3f(-200.0f, 0.0f,  200.0f);
	glVertex3f( 200.0f, 0.0f,  200.0f);
	glVertex3f( 200.0f, 0.0f, -200.0f);
	glEnd();
}

void drawHUD() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	//glVertex3f(x-3, -0.4f, z-4);
	//glVertex3f(x-3, 0.4f, z-4);
	//glVertex3f(x+3, 0.4f, z-4);
	//glVertex3f(x+3, -0.4f, z-4);
	const float dist_to_hud = 4.0;
	const float half_hud_width = 3.0;

	float H = half_hud_width / sin(angle);
	float hudz = dist_to_hud * sin(angle);		//stick HUD to camera
	glVertex3f(x+H, 0.0f, z - hudz);
	glVertex3f(x+H, y/4.0f, z - hudz);
	glVertex3f(x-H, y/4.0f, z - hudz);
	glVertex3f(x-H, 0.0f, z - hudz);

/*
	float hudx = dist_to_hud * cos(angle);
	float hudz = dist_to_hud * sin(angle);

	glVertex3f(x+hudx-half_hud_width, 0.0f, z - hudz);
	glVertex3f(x+hudx-half_hud_width, y/4.0f, z - hudz);
	glVertex3f(x+hudx+half_hud_width, y/4.0f, z - hudz);
	glVertex3f(x+hudx+half_hud_width, 0.0f, z - hudz);
*/
	glEnd();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGround();
	drawHUD();
	drawSphere();
	// Draw 36 Wise Men

	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*40.0,0,j * 40.0);
			if (checkMaze((i + 3), (j + 3)) == 1)
			{
				drawHorizontalWall();
			}
			if (checkMaze((i + 3), (j + 3)) == 2)
			{
				drawWiseMen();
			}
			//glCallList(snowman_display_list);;
			glPopMatrix();
		}
		glutSwapBuffers();
}

void orientMe(float ang) {


	//lx = sin(ang);
	//lz = -cos(ang);
	angle -= ang;

	x = spherePosX + CAM_DIST * cos(angle);
	z = spherePosZ + CAM_DIST * sin(angle);

	//lx = x - oldx;
	//lz = z - oldz;

	moveCamera(x, y, z);
	/*
	glLoadIdentity();
	gluLookAt(x, y, z, 
		//x + lx,y + ly,z + lz,		//original
		spherePosX, spherePosY, spherePosZ,		//always look at sphere
		0.0f,1.0f,0.0f);
	*/
}

void moveMeFlat(int i) {
	//x = x + i*(lx)*0.1;
	//z = z + i*(lz)*0.1;

	//new camera coords:
	z = z + i * sin(angle);
	x = x + i * cos(angle);
	//new sphere coords:
	spherePosX = spherePosX + i * cos(angle);	//sphere moves in a straight line in the direction of the camera angle
	spherePosZ = spherePosZ + i * sin(angle);

	moveCamera(x, y, z);
	/*
	glLoadIdentity();
	gluLookAt(x, y, z, 
		x + lx,y + ly,z + lz,
		0.0f,1.0f,0.0f);
	*/
}

void processNormalKeys(unsigned char key, int x, int y) {

	if ((key == 27) || (key = 'q') || (key = 'Q'))
		exit(0);
}


void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
			orientMe(-TURN_ANGLE);
			//sphereRotX = (sphereRotX - 5) % 360;
			break;
		case GLUT_KEY_RIGHT : 
			orientMe(TURN_ANGLE);
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
	glutInitWindowSize(640,360);
	glutCreateWindow("SPHEREQUEST");
	
	//Get the maze set for the level
	generateMaze();
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