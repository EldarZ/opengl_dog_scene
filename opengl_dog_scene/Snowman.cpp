#include "Snowman.h"

void Snowman::draw()
{
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Draw Body	
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}