#include "Table.h"
#include <GL\freeglut.h>


Table::Table()
{
}

void Table::draw()
{
	glPushMatrix();
	GLfloat color[3] = { 0.651, 0.502, 0.392 };
	glColor3fv(color);

	GLfloat table_ambient[] = { 0.001f, 0.001f, 0.001f },
		table_diffuse[] = { 0.0001f, 0.0001f, 0.0001f },
		table_specular[] = { 0.1f, 0.1f, 0.1f },
		table_shininess = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, table_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, table_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, table_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, table_shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

	//scale to model with higher values
	glScalef(0.3, 0.3, 0.3);
	glTranslated(-7, 3.5, -7);

	glPushMatrix();
	glScalef(7, 0.5, 7);
	glutSolidCube(1);
	glPopMatrix();

	//torso
	glPushMatrix();
	glTranslated(-2.5, -1.75, -2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	//legs
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


Table::~Table()
{
}
