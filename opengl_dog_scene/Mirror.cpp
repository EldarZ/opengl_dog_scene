#include "Mirror.h"

Mirror::Mirror() :color{ 1, 1, 1, 0.5f } {};

void Mirror::draw() {
	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f },
		diffuse[] = { 0.9f, 0.9f, 0.9f },
		specular[] = { 1.0f, 1.0f, 1.0f },
		shininess = 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glBegin(GL_QUADS);

	GLfloat step = 0.1f;
	for (GLfloat x = 0; x < 1.5; x += step) {
		for (GLfloat y = 0; y < 2.5; y += step) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
			glVertex3d(x, y, 0);
			glVertex3d(x + step, y, 0);
			glVertex3d(x + step, y + step, 0);
			glVertex3d(x, y + step, 0);
		}
	}
	glEnd();
}