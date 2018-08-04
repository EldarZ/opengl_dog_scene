#pragma once
#include <math.h>

class Spotlight {
public:
	GLfloat position[4] = { 0.0f, 2.2f, 0.0f , 1.0f };
	GLfloat direction[3] = { 0.1f, 0.0f, 0.0f };
	GLfloat spotlightColor[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat cutoff = 30.0f;
	GLfloat exponent = 0.0f;
	void enable()
	{
		glEnable(GL_LIGHT1);
	}

	void draw() {
		glPushMatrix();

		glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlightColor);
		glLightfv(GL_LIGHT1, GL_SPECULAR, spotlightColor);
		glLightfv(GL_LIGHT1, GL_POSITION, position);
		GLfloat direction2[3] = { direction[0] - position[0], 
							      direction[1] - position[1], 
							      direction[2] - position[2] };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction2);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	
		glDisable(GL_LIGHTING);
		glColor3fv(spotlightColor);
		glutSolidSphere(0.2, 100, 100);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	void disable()
	{
		glDisable(GL_LIGHT1);
	}
};