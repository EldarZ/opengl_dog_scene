#pragma once
#include <GL\freeglut.h>

/*
Light is directional global lighting object
*/
class PointLight
{
public:
	GLfloat color[3] = { 0.691f, 0.653f, 0.254f };
	GLfloat position[4] = { 5.0f, 3.0f, 0.0f , 1.0f };

	void draw()
	{
		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		//The initial spot cutoff is 180, resulting in uniform light distribution.
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);

		glDisable(GL_LIGHTING);
		glColor3fv(color);
		glutSolidSphere(0.2, 100, 100);
		glEnable(GL_LIGHTING);

		glPopMatrix();
	}

	void disable()
	{
		glDisable(GL_LIGHT0);
	}

	void enable()
	{
		glEnable(GL_LIGHT0);
	}
};

