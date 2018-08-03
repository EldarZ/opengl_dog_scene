#pragma once
#include <GL\freeglut.h>

class Light
{
public:
	GLfloat position[4] = { 0.0f, 5.0f, 0.0f , 0.0f };
	GLfloat intensity = 0.3f;
	Light() {};

	void enable()
	{
		glEnable(GL_LIGHT0);
	}

	void draw()
	{
		glPushMatrix();
		GLfloat lightColor[] = { intensity , intensity , intensity };

		glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		glPopMatrix();
	}

	void disable()
	{
		glDisable(GL_LIGHT0);
	}


	~Light() {};
};

