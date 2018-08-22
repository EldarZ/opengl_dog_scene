#include "Dog.h"
#include <GL\freeglut.h>

Dog::Dog() :headHorizontalAngle(0.0f),
            headVerticalAngle(10.0f),
            tailHorizontalAngle(0.0f),
            tailVerticalAngle(-10.0f),
            tailWiggleAngle(0.0f),
            tailWiggleDirectionLeft(true),
            legsAngle(0.0f),
            legsMovementDirectionForward(true),
            nextMove(nullptr),
            isMoving(false) {};

void Dog::init() {
	GLfloat viewModelMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
	glLoadIdentity();

	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(-0.5, 3.5f * 0.30f, -2.8f);

	glGetFloatv(GL_MODELVIEW_MATRIX, local);
	glLoadMatrixf(viewModelMatrix);
}

void Dog::draw() {
	updateConstantMovement();

	glPushMatrix();
	GLfloat color[4] = { 0.92f, 0.814f, 0.382f, 1.0f };
	glColor4fv(color);

	GLfloat dog_specular[] = { 0.1f, 0.1f, 0.1f },
			dog_shininess = 0.1f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, dog_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, dog_shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

	//torso
	glPushMatrix();
	glScalef(2.0f * 0.3f, 2.0f * 0.3f, 4.0f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//legs
	glPushMatrix();
	glRotatef(legsAngle, 1, 0, 0);
	glTranslated(-1 * 0.3, -2.5 * 0.3, -2 * 0.3);
	glScalef(0.5f * 0.3f, 2.0f * 0.3f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-legsAngle, 1, 0, 0);
	glTranslated(0.3f, -2.5f * 0.3f, -0.6);
	glScalef(0.5f * 0.3f, 0.6f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glRotatef(legsAngle, 1, 0, 0);
	glTranslated(0.3f, -2.5f * 0.3f, 2.0 * 0.3f);
	glScalef(0.5f * 0.3f, 2.0f * 0.3f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-legsAngle, 1, 0, 0);
	glTranslated(-0.3f, -2.5f * 0.3f, 0.6);
	glScalef(0.5f * 0.3f, 2.0f * 0.3f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//tail
	glPushMatrix();
	glTranslated(0.0f, 0.0f, -3.8f * 0.3f);
	glRotatef(-30, 1, 0, 0);
	glRotatef(tailVerticalAngle, 1, 0, 0);
	glRotatef(tailHorizontalAngle, 0, 1, 0);
	glRotatef(tailWiggleAngle, 0, 1, 0);
	glScalef(0.5f * 0.3f, 0.5f * 0.3f, 1.8f * 0.3f);

	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//head rotation
	glPushMatrix();
	glRotatef(headVerticalAngle, 1, 0, 0);
	glRotatef(headHorizontalAngle, 0, 1, 0);

	//head
	glPushMatrix();
	glTranslated(0.0f, 2.5f * 0.3f, 3.0f * 0.3f);
	glScalef(1.5f * 0.3f, 1.55f * 0.3f, 1.6f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//nose
	glPushMatrix();
	glTranslated(0.0f, 2.2f * 0.3f, 4.2f * 0.3f);
	glScalef(0.8f * 0.3f, 0.5f * 0.3f, 1.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//ears
	glPushMatrix();
	glTranslated(-0.8f * 0.3f, 3.8f * 0.3f, 2.6f * 0.3f);
	glScalef(0.5f * 0.3f, 0.3f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8f * 0.3f, 3.8f * 0.3f, 2.6f * 0.3f);
	glScalef(0.5f * 0.3f, 1.0f * 0.3f, 0.5f * 0.3f);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	//eyes
	GLfloat eyes_specular[] = { 0.4f, 0.4f, 0.4f },
			eyes_shininess = 1.0f;
	GLfloat black[] = { 0,0,0,1};
	glColor4fv(black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, eyes_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, eyes_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);

	glPushMatrix();
	glTranslated(0.5f * 0.3f, 3.0f * 0.3f, 4.4f * 0.3f);
	glScalef(0.25f * 0.3f, 0.25f * 0.3f, 0.25f * 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5f * 0.3f, 3.0f * 0.3f, 4.4f  * 0.3f);
	glScalef(0.25f * 0.3f, 0.25f * 0.3f, 0.25f * 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void Dog::updateConstantMovement() {
	if (tailWiggleAngle > 8 || tailWiggleAngle < -8)
	{
		tailWiggleDirectionLeft = !tailWiggleDirectionLeft;
	}
	if (tailWiggleDirectionLeft)
	{
		tailWiggleAngle += 1.7f;
	}
	else {
		tailWiggleAngle -= 1.7f;
	}
	if (isMoving) {
		if (legsAngle > 20 || legsAngle < -20)
		{
			legsMovementDirectionForward = !legsMovementDirectionForward;
		}
		if (legsMovementDirectionForward)
		{
			legsAngle += 6.0;
		}
		else {
			legsAngle -= 6.0;
		}
		isMoving = false;
	}
}