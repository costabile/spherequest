#include "draw.h"
#include "player.h"
#include <math.h>
draw::draw(){}

//void draw::drawSphere() {
//	glPushMatrix();
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glTranslatef(spherePosX,spherePosY,spherePosZ);
//	glRotatef(sphereRotAng, sphereRotX, sphereRotY, sphereRotZ);
//	glutSolidSphere(SPHERE_RAD, 20, 20);
//	glPopMatrix();
//}
float PI = 3.1415926535897932384626433832795;
float x = 0;

void draw::drawTemples(GLuint roofTexture, GLuint redTexture) {

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	
	glColor3f(0.7, 0.3, 0.3);
	glPushMatrix();

	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

	
	//Draw Roof
	glBindTexture( GL_TEXTURE_2D, roofTexture);
	glDisable(GL_CULL_FACE);		//don't cull the roof (both sides are visible)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(20,-20,32);	//orig: 0, 0, 32 (shifted to draw the temple within the grid cell)
	glutSolidCone(40, 30, 20, 20);
	glPopMatrix();
	glEnable(GL_CULL_FACE);

	glBindTexture( GL_TEXTURE_2D, redTexture);
	glTranslatef(20, 20, 20);	//orig: 0,20,0
	glutSolidCube(40);

	//Draw Roof
	glBindTexture( GL_TEXTURE_2D, roofTexture);
	glDisable(GL_CULL_FACE);		//don't cull the roof (both sides are visible)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,45);
	glutSolidCone(30, 20, 20, 20);
	glPopMatrix();
	glEnable(GL_CULL_FACE);

	glBindTexture( GL_TEXTURE_2D, redTexture);
	glTranslatef(0, 35, 0);
	glutSolidCube(30);

	//Draw Roof
	glBindTexture( GL_TEXTURE_2D, roofTexture);
	glDisable(GL_CULL_FACE);		//don't cull the roof (both sides are visible)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,35);
	glutSolidCone(20, 10, 20, 20);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	
	glBindTexture( GL_TEXTURE_2D, redTexture);
	glTranslatef(0, 25, 0);
	glutSolidCube(20);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_2D);

}

void draw::drawXWall() {

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	//glTranslatef(-10, 10, 0);
	glTranslatef(10, 10, 20);

	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

	glutSolidCube(20);
	glTranslatef(20, 0, 0);
	glutSolidCube(20);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_2D);
}

void draw::drawZWall() {

	glColor3f(0.5, 0.5, 0.5);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glPushMatrix();
	//glTranslatef(0, 10, -10);
	glTranslatef(20, 10, 10);

	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

	glutSolidCube(20);
	glTranslatef(0, 0, 20);
	glutSolidCube(20);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_2D);
}

void draw::drawTree() {
	glDisable(GL_CULL_FACE);		//don't cull the tree (inside of cones are visible)
	
	float transx;
	float max = 2 * PI;
	// Draw Body		
	if (x <= max){
		x += 0.01;
		transx = sin(x);
	}
	else{
		x = 0;
		transx = sin(x);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();

	GLfloat dull[3] = {0, 0, 0};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

	glRotatef(-90, 1, 0, 0);
	//glTranslatef(0, 0, 0);
	glTranslatef(20, -20, 0);
	
	glPushMatrix();
	glRotatef(-2 * transx, 1, 0, 0);
	glutSolidCone(3, 5, 10, 10);
	glPopMatrix();
	
	glTranslatef(0, 0, 2.5);
	
	glPushMatrix();
	glRotatef(-3 * transx, 1, 0, 0);
	glutSolidCone(3, 5, 10, 10);
	glPopMatrix();

	glTranslatef(0, 0, 2.5);
	
	glPushMatrix();
	glRotatef(-4 * transx, 1, 0, 0);
	glutSolidCone(3, 5, 10, 10);
	glPopMatrix();
	
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}

void draw::drawWiseMen() {
	
	glDisable(GL_CULL_FACE);

	float transx;
	float max = 2 * PI;
	// Draw Body		
	if (x <= max){
		x += 0.01;
		transx = sin(x);
	}
	else{
		x = 0;
		transx = sin(x);
	}
	glTranslatef(0, 1*transx + 2.5, 0);
	glPushMatrix();
	GLfloat dull[3] = {1, 1, 1};
	glMaterialfv (GL_FRONT, GL_SPECULAR , dull);

	glColor3f(1.0f, 1.0f, 0.5f);
	//glTranslatef(0.0f ,0.0f, 0.0f);
	glTranslatef(20.0f ,0.0f, 20.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.6f,2.0f,20,20);
	glPopMatrix();

	glEnable(GL_CULL_FACE);

	//Draw Belt
	glPushMatrix();
	glColor3f(0.5f, 0.25f, 0.0f);
	//glTranslatef(0.0f ,0.7f, 0.0f);
	glTranslatef(20.0f ,0.7f, 20.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(0.19f, 0.3f,10,10);
	glPopMatrix();

	// Draw Head
	glColor3f(1.0f, 0.75f, 0.6f);
	//glTranslatef(0.0f, 1.8f, 0.0f);
	glTranslatef(20.0f, 1.8f, 20.0f);
	glutSolidSphere(0.35f,20,20);
	
	//Draw Beard
	glPushMatrix();
	glColor3f(0.5f,0.5f,0.5f);
	glTranslatef(0.0f, -0.25f, 0.05f);
	glRotatef(50.0f + (5 * transx), 1.0f, 0.0f, 0.0f);
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
	glDisable(GL_CULL_FACE);		//don't cull the hate (both sides are visible)
	glPushMatrix();
	glTranslatef(0.0f, 0.2f + (0.1*transx), 0.0f);
	glColor3f(1.0f, 0.8f , 0.2f);
	glRotatef(-90.0f ,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.9f,0.35f,10,2);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

void draw::drawGround(){
	// Draw ground
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-200.0f, 0.0f, -200.0f);
	glTexCoord2f(50, 0);
	glVertex3f(-200.0f, 0.0f,  200.0f);
	glTexCoord2f(50, 50);
	glVertex3f( 200.0f, 0.0f,  200.0f);
	glTexCoord2f(0, 50);
	glVertex3f( 200.0f, 0.0f, -200.0f);
	glEnd();
}

//Sky is pretty
void draw::drawSkybox(){
	glCullFace(GL_FRONT);		//invert culling (because we see the inside of the skybox)

	glColor3f(0.8, 0.9, 1.0);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidSphere(300, 10, 10);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glCullFace(GL_BACK);		//restore normal culling
}

//Couldn't resist. May put this in skybox texture later
void draw::drawMtFuji() {
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 400, 0);
	glutSolidCone(200, 200, 20, 20);
	glPopMatrix();
}