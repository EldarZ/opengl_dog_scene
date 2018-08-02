#pragma once
#include <GL\freeglut.h>

class Light
{
public:
	GLfloat _temp;
	Light() : _temp(3.0f){};

	void init()
	{
		glEnable(GL_LIGHT0);
	}

	void draw()
	{
		

		glPushMatrix();

		GLfloat ambient[3] = { 0.1f, 0.1f, 0.1f };
		GLfloat color[4] = { 0.9f, 0.9f, 0.9f, 0.9f };
		GLfloat position[4] = { -3.0, _temp,3.0 , 0.0 };

		glLightfv(GL_LIGHT0, GL_POSITION, position);

		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

		glColor3fv(color);
		GLfloat ambient1[] = { 10.0, 10.0, 10.0, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
		glTranslatef(position[0], position[1], position[2]);
		glutSolidSphere(0.2, 100, 100);

		glPopMatrix();
	}

	~Light() {};
};

