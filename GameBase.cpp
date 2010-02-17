#include <glut.h>
#include <math.h>
//#include <gl.h>
#include <glu.h>

static float angle=0.0, ratio;
static float x=0.0f, y=1.75f, z=5.0f;
static float lx=0.0f,ly=0.0f, lz=-1.0f;
static GLint snowman_display_list;

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
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void drawSnowMan() {


// Draw Body	
	glColor3f(0.0f, 0.0f, 0.6f);
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);


// Draw Head
	glColor3f(1.0f, 0.65f, 0.65f);
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.15f, 0.0f, 0.2f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Hat
	glTranslatef(0.0f, 0.15f, 0.0f);
	glColor3f(0.7f, 0.0f , 0.0f);
	glRotatef(-90.0f ,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.25f,0.7f,10,2);

//Draw Skybox
	glColor3f(0.8f, 0.8f , 1.0f);
	glutSolidSphere(100.00f,10,10);
}

GLuint createDL() {
	GLuint snowManDL;

	// Create the id for the list
	snowManDL = glGenLists(1);

	// start list
	glNewList(snowManDL,GL_COMPILE);

	// call the function that contains the rendering commands
		drawSnowMan();

	// endList
	glEndList();

	return(snowManDL);
}

void initScene() {

	glEnable(GL_DEPTH_TEST);
	snowman_display_list = createDL();

}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Draw ground

	glColor3f(0.0f, 0.6f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

// Draw 36 SnowMen

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


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}


void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : angle -= 0.1f;orientMe(angle);break;
		case GLUT_KEY_RIGHT : angle +=0.1f;orientMe(angle);break;
		case GLUT_KEY_UP : moveMeFlat(7);break;
		case GLUT_KEY_DOWN : moveMeFlat(-7);break;
	}
}

void drawTicker(void) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(100., 0.0, 0.0);
		glVertex3f(100.0, 100.0, 0.0);
		glVertex3f(0.0, 100.0, 0.0);
	glEnd();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("GARDEN GNOME FLIGHT SIMULATOR!!!!!");

	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutDisplayFunc(drawTicker);

	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}