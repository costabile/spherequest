//#include <glut.h>
#include <math.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>

#define CAM_DIST 20.0		//distance camera keeps from sphere
#define PI 3.14159265358979323846

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;
static float lx=0.0f,ly=0.0f, lz=-1.0f;
static GLint snowman_display_list;

//sphere position:
GLfloat spherePosX = 0.0;
GLfloat spherePosY = 2.0;
GLfloat spherePosZ = -15.0;
//sphere rotation:
int sphereRotX = 0.0;
int sphereRotY = 0.0;
int sphereRotZ = 0.0;
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

void drawSphere() 
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(spherePosX,spherePosY,spherePosZ);
	glRotatef(sphereRotAng, sphereRotX, sphereRotY, sphereRotZ);
	glutSolidSphere(2.0f, 20, 20);
	glPopMatrix();

	//draw cone...for testing purposes, to know how the sphere is oriented.
	glPushMatrix();
	glTranslatef(spherePosX,spherePosY + 2.0,spherePosZ);
	glRotatef(sphereRotAng, sphereRotX, sphereRotY, sphereRotZ+180);
	glColor3f(1.0f, 1.0f , 1.0f);
	glutSolidCone(0.9f,0.35f,10,2);
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
	glPushMatrix();
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
	glPopMatrix();

}

void drawGround(){
	// Draw ground
	glColor3f(0.2f, 0.7f, 0.2f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f,  100.0f);
	glVertex3f( 100.0f, 0.0f,  100.0f);
	glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
}

void drawHUD() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-1, -0.4f, z-4);
	glVertex3f(-1, 0.4f, z-4);
	glVertex3f( 1, 0.4f, z-4);
	glVertex3f( 1, -0.4f, z-4);
	glEnd();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGround();
	drawHUD();
	drawSphere();
	// Draw 36 Gnomes

	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			glCallList(snowman_display_list);;
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
			orientMe(-PI/16.0f);
			//sphereRotX = (sphereRotX - 5) % 360;
			break;
		case GLUT_KEY_RIGHT : 
			orientMe(PI/16.0f);
			//sphereRotX = (sphereRotX + 5) % 360;
			break;
		case GLUT_KEY_UP :
			moveMeFlat(-7);
			//sphereRotY = (sphereRotY + 5) % 360;
			break;
		case GLUT_KEY_DOWN : 
			moveMeFlat(7);
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