#include "Dog.h"
#include <GL\freeglut.h>

void Dog::draw() {
	updateConstantMovement();

	glPushMatrix();
	GLfloat color[3] = { 0.92, 0.814, 0.382 };
	glColor3fv(color);

	GLfloat dog_ambient[] = { 0.01f, 0.01f, 0.01f },
		dog_diffuse[] = { 0.1f, 0.1f, 0.1f },
		dog_specular[] = { 0.1f, 0.1f, 0.1f },
		dog_shininess = 0.3f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, dog_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dog_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, dog_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, dog_shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);

	//scale to model with higher values
	glScalef(0.3, 0.3, 0.3);
	glTranslated(0, 3.5, 0);

	//torso
	glPushMatrix();
	glScalef(2, 2, 4);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//legs
	glPushMatrix();
	glTranslated(-1, -2.5, -2);
	glScalef(0.5, 2, 0.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, -2.5, -2);
	glScalef(0.5, 2, 0.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, -2.5, 2);
	glScalef(0.5, 2, 0.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, -2.5, 2);
	glScalef(0.5, 2, 0.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//tail
	glPushMatrix();
	glTranslated(0, 0, -3.8);
	glRotatef(-30, 1, 0, 0);
	glRotatef(tailVerticalRotation, 1, 0, 0);
	glRotatef(tailSideRotation, 0, 1, 0);
	glRotatef(tailContinuesSideRotation, 0, 1, 0);
	glScalef(0.5, 0.5, 1.8);

	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//head rotation
	glPushMatrix();
	glRotatef(headVerticalRotation, 1, 0, 0);
	glRotatef(headSideRotation, 0, 1, 0);

	//head
	glPushMatrix();
	glTranslated(0, 2.5, 3);
	glScalef(1.5, 1.55, 1.6);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//nose
	glPushMatrix();
	glTranslated(0, 2.2, 4.2);
	glScalef(0.8, 0.5, 1.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//ears
	glPushMatrix();
	glTranslated(-0.8, 3.8, 2.6);
	glScalef(0.5, 1, 0.5);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8, 3.8, 2.6);
	glScalef(0.5, 1, 0.5);
	glutSolidSphere(1, 30, 30);
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
	glTranslated(0.5, 3.0, 4.4);
	glScalef(0.25, 0.25, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, 3.0, 4.4 );
	glScalef(0.25, 0.25, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
