#include "stdafx.h"
#include <GL/glut.h>
#include <cmath>
#include <ctime>

bool bOne = true,
bTwo = false;

float fAngle = 0.0;

float fEyeX = 0.0f, fEyeY = 1.75f, fEyeZ = 5.0f;
float fCenterX = 0.0f, fCenterY = 0.0f, fCenterZ = -1.0f;

float fRatio = 1.0;

void DrawSnowMan();

void Reshape(int width, int height)
{
	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0)
		height = 1;

	fRatio = 1.0f * width / height;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(45, fRatio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(fEyeX, fEyeY, fEyeZ,
		fEyeX + fCenterX, fEyeY + fCenterY, fEyeZ + fCenterZ,
		0.0f, 1.0f, 0.0f);
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw ground

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Draw 36 SnowMen
	//glTranslatef (0, 1.2, 0);

	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10.0, 1.2, j * 10.0);
			DrawSnowMan();
			glPopMatrix();
		}


	glutSwapBuffers();
	glFlush();
}

void OrientMe(float ang)
{
	fCenterX = sin(ang);
	fCenterZ = -cos(ang);

	glLoadIdentity();

	gluLookAt(fEyeX, fEyeY, fEyeZ,
		fEyeX + fCenterX, fEyeY + fCenterY, fEyeZ + fCenterZ,
		0.0f, 1.0f, 0.0f);
}

void MoveMeFlat(int i)
{
	fEyeX = fEyeX + i * (fCenterX) * 0.1;
	fEyeZ = fEyeZ + i * (fCenterY) * 0.1;

	glLoadIdentity();

	gluLookAt(fEyeX, fEyeY, fEyeZ,
		fEyeX + fCenterX, fEyeY + fCenterY, fEyeZ + fCenterZ,
		0.0f, 1.0f, 0.0f);
}

void ProcessSpecialKeys(int key, int x, int y) {

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		fAngle -= 0.01f;
		OrientMe(fAngle); break;
	case GLUT_KEY_LEFT:
		fAngle += 0.01f;
		OrientMe(fAngle); break;
	case GLUT_KEY_UP:
		MoveMeFlat(1); break;
	case GLUT_KEY_DOWN:
		MoveMeFlat(-1); break;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("SnowMen");

	Init();

	glutSpecialFunc(ProcessSpecialKeys);

	glutDisplayFunc(Display);
	glutIdleFunc(Display);

	glutReshapeFunc(Reshape);

	glutMainLoop();
}

void DrawSnowMan()
{
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
}