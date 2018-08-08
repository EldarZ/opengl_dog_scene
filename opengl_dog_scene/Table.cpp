#include "Table.h"
#include <GL\freeglut.h>

void Table::draw()
{
	glPushMatrix();
	GLfloat color[4] = { 0.651f, 0.502f, 0.392f, 1.0f};
	glColor4fv(color);

	GLfloat specular[] = { 0.1f, 0.1f, 0.1f },
			shininess = 0.3f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); 

	//scale to model with higher values
	glScalef(0.3f, 0.3f, 0.3f);

	//top
	glPushMatrix();
	glScalef(7, 0.5, 7);
	glutSolidCube(1);
	glPopMatrix();

	//legs
	glPushMatrix();
	glTranslated(-2.5, -1.75, -2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.5, -1.75, -2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, -1.75, 2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.5, -1.75, 2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}