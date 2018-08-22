#include "Teapot.h"

void Teapot::draw() {
	glPushMatrix();
	GLfloat ambient[] = { 0.19225f,	0.19225f,	0.19225f, 1.0f },
		diffuse[] = { 0.50754f,	0.50754f,	0.50754f, 1.0f },
		specular[] = { 0.508273f,	0.508273f,	0.508273f },
		shininess = 0.4f * 128.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glutSolidTeapot(0.3);

	glPopMatrix();
}