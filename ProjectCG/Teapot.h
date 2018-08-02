#pragma once
#include <GL\freeglut.h>

class Teapot {
public:
	Teapot()  {};
	void draw() {
		glPushMatrix();
		glTranslated(-2, 1.35, -2);
		
		GLfloat teapot_ambient[] = { 0.19225,	0.19225,	0.19225 },
			    teapot_diffuse[] = { 0.50754,	0.50754,	0.50754 },
			    teapot_specular[] = { 0.508273,	0.508273,	0.508273 },
			    teapot_shininess = 0.4f;

		glMaterialfv(GL_FRONT, GL_AMBIENT,  teapot_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,  teapot_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, teapot_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, teapot_shininess * 128.0);
		//glMaterialf(GL_FRONT, GL_EMISSION, 0);

		glutSolidTeapot(0.3);

		glPopMatrix();
	}
};
