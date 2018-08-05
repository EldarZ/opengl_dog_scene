#pragma once
#include <GL\freeglut.h>

/*
Opengl original teapot.
*/
class Teapot {
public:
	Teapot()  {};
	void draw() {
		glPushMatrix();
		GLfloat teapot_ambient[] = { 0.19225,	0.19225,	0.19225 },
			    teapot_diffuse[] = { 0.50754,	0.50754,	0.50754 },
			    teapot_specular[] = { 0.508273,	0.508273,	0.508273 },
			    teapot_shininess = 0.4f * 128.0;

		glMaterialfv(GL_FRONT, GL_AMBIENT,  teapot_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,  teapot_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, teapot_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, teapot_shininess);

		glutSolidTeapot(0.3);

		glPopMatrix();
	}
};
