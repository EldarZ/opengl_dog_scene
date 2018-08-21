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
		GLfloat up[3] = { 0, 1, 0 };
		lookAt(position, target, up);

		GLfloat ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat diffuse[4] = { 0.01f, 0.01f, 0.01f, 1.0f };
		GLfloat specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat shininess = 32.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);

		glutSolidCone(0.3, 0.6, 10, 10);
		glPushMatrix();
		glTranslatef(0, 0, 0.1);
		glutSolidCylinder(0.2, 0.39, 10, 10);
		glPopMatrix();

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

	void normalize(const GLfloat* vec, GLfloat* output)
	{
		GLfloat length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
		output[0] /= length;
		output[1] /= length;
		output[2] /= length;
	}

	void cross(const GLfloat* vec1, const GLfloat* vec2, GLfloat * output) {
		output[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
		output[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
		output[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	}

	void lookAt(GLfloat* eye, GLfloat* center, GLfloat* up)
	{
		GLfloat f[3] = { center[0] - eye[0],
						 center[1] - eye[1],
						 center[2] - eye[2] };

		normalize(f, f);
		GLfloat u[3];
		normalize(up, u);
		GLfloat s[3];
		cross(f, u, s);
		normalize(s, s);
		cross(s, f, u);
		normalize(u, u);

		GLfloat rotationMatrixFromOrientation[16];
		rotationMatrixFromOrientation[0] = s[0];
		rotationMatrixFromOrientation[1] = s[1];
		rotationMatrixFromOrientation[2] = s[2];
		rotationMatrixFromOrientation[3] = 0;

		rotationMatrixFromOrientation[4] = u[0];
		rotationMatrixFromOrientation[5] = u[1];
		rotationMatrixFromOrientation[6] = u[2];
		rotationMatrixFromOrientation[7] = 0;

		rotationMatrixFromOrientation[8] =  -f[0];
		rotationMatrixFromOrientation[9] =  -f[1];
		rotationMatrixFromOrientation[10] = -f[2];
		rotationMatrixFromOrientation[11] = 0;

		rotationMatrixFromOrientation[12] = 0;
		rotationMatrixFromOrientation[13] = 0;
		rotationMatrixFromOrientation[14] = 0;
		rotationMatrixFromOrientation[15] = 1;

		glMultMatrixf(rotationMatrixFromOrientation);	
	}
};