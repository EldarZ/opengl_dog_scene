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
		GLfloat teapot_ambient[] = { 0.19225f,	0.19225f,	0.19225f },
			    teapot_diffuse[] = { 0.50754f,	0.50754f,	0.50754f },
			    teapot_specular[] = { 0.508273f,	0.508273f,	0.508273f },
			    teapot_shininess = 0.4f * 128.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT,  teapot_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,  teapot_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, teapot_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, teapot_shininess);

		glutSolidTeapot(0.3);

		glPopMatrix();
	}
};
