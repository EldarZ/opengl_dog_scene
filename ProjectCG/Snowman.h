#pragma once
#include <GL/freeglut.h>


class Snowman {
public:
	void draw()
	{
		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw Body	
		glTranslatef(0.0f, 0.75f, 0.0f);
		glutSolidSphere(0.75f, 20, 20);

		// Draw Head
		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f, 20, 20);

		// Draw Eyes
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f, 10, 10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

		// Draw Nose
		glColor3f(1.0f, 0.5f, 0.5f);
		glutSolidCone(0.08f, 0.5f, 10, 2);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
	}
};
