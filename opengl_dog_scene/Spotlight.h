#pragma once
#include <math.h>

/*
Spotlight object handling position, target and other lighting properties.
*/
class Spotlight {
public:
	GLfloat position[4] = { -1.0f, 2.5f, 0.0f , 1.0f };
	GLfloat target[3] = { 0.1f, 0.0f, 0.0f };
	GLfloat color[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat cutoff = 30.0f;
	GLfloat exponent = 0.0f;

	void addlight() {
		if (!glIsEnabled(GL_LIGHT1))
			return;
		glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT1, GL_SPECULAR, color);
		glLightfv(GL_LIGHT1, GL_POSITION, position);
		GLfloat direction[3] = { target[0] - position[0],
			target[1] - position[1],
			target[2] - position[2] };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	}

	void draw() {
		if (!glIsEnabled(GL_LIGHT1))
			return;
	
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3fv(color);
		glutSolidSphere(0.2, 100, 100);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	void disable()
	{
		glDisable(GL_LIGHT1);
	}

	void enable()
	{
		glEnable(GL_LIGHT1);
	}
};