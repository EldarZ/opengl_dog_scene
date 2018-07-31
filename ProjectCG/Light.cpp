#include "Light.h"


void Light::draw() {
	position[1] = _temp;

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glColor3fv(color);
	GLfloat ambient[] = { 10.0, 10.0, 10.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(0.2, 100, 100);

	glPopMatrix();
}

