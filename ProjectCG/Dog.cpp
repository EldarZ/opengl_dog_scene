#include "Dog.h"
#include <GL\freeglut.h>


Dog::Dog()
{
}

void Dog::draw() {
	glPushMatrix();
	GLfloat color[3] = { 0.92, 0.814, 0.382 };
	glColor3fv(color);

	GLfloat dog_ambient[] = { 0.001f, 0.001f, 0.001f },
		dog_diffuse[] = { 0.0001f, 0.0001f, 0.0001f },
		dog_specular[] = { 0.1f, 0.1f, 0.1f },
		dog_shininess = -1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, dog_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dog_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, dog_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, dog_shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

	//scale to model with higher values
	glScalef(0.3, 0.3, 0.3);
	glTranslated(0, 3.5, 0);

	glPushMatrix();
	glScalef(3, 3, 5);
	glutSolidCube(1);
	glPopMatrix();

	//torso
	glPushMatrix();
	glTranslated(-1, -2.5, -2);
	glScalef(1, 2, 1);
	glutSolidCube(1);
	glPopMatrix();

	//legs
	glPushMatrix();
	glTranslated(1, -2.5, -2);
	glScalef(1, 2, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, -2.5, 2);
	glScalef(1, 2, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, -2.5, 2);
	glScalef(1, 2, 1);
	glutSolidCube(1);
	glPopMatrix();

	//tail
	glPushMatrix();
	glTranslated(0, 0.5, -3.3);
	glRotatef(-30, 1, 0, 0);
	glScalef(0.5, 0.5, 1.8);

	glutSolidCube(1);
	glPopMatrix();

	//head rotation
	glPushMatrix();
	glRotatef(10, 1, 0, 0);

	//head
	glPushMatrix();
	glTranslated(0, 2.5, 3);
	glScalef(2, 2.1, 2.2);
	glutSolidCube(1);
	glPopMatrix();

	//nose
	glPushMatrix();
	glTranslated(0, 2.2, 4);
	glScalef(1.5, 1, 2);
	glutSolidCube(1);
	glPopMatrix();

	//ears
	glPushMatrix();
	glTranslated(-0.8, 3.8, 2.6);
	glScalef(0.5, 1, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8, 3.8, 2.6);
	glScalef(0.5, 1, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	//eyes
	GLfloat eyes_ambient[] = { 0.02f, 0.02f, 0.02f },
		eyes_diffuse[] = { 0.02f, 0.01f, 0.01f },
		eyes_specular[] = { 0.4f, 0.4f, 0.4f },
		eyes_shininess = .0;
	GLfloat black[] = { 0,0,0 };
	glColor3fv(black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, eyes_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, eyes_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, eyes_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, eyes_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslated(0.5, 3.0, 4.1);
	glScalef(0.25, 0.25, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, 3.0, 4.1);
	glScalef(0.25, 0.25, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}


Dog::~Dog()
{
}
