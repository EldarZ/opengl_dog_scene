#include "stdafx.h"

#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>
using namespace std;

static int WINDOW_WIDTH = 400;
static int WINDOW_HEIGHT = 400;
static float PI = 3.1415926535;

void handleMouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		int currentScale = std::fmin(WINDOW_WIDTH, WINDOW_HEIGHT);
		//Exit rect location : (0.89, 0.0, 1.0, 0.05);
		bool isXInExitRect = x < currentScale && x > currentScale - currentScale * 0.11;
		bool isYInExitRect = y < currentScale && y > currentScale - currentScale * 0.05;
		if (state == GLUT_DOWN && isXInExitRect && isYInExitRect) {
			exit(0);
		}
		break;
	}
}

void handleResize(int w, int h) {
	int newScale = std::fmin(w, h);
	glViewport(0, h - newScale, newScale, newScale);
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
}

void drawText(GLfloat x, GLfloat y, string text, void *font) {
	glRasterPos2f(x, y);
	for (unsigned int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(font, text[i]);
	}
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	//sun
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (GLfloat angle = 0; angle <= 2 * PI; angle += 0.01)
		glVertex2f(0.75f + cos(angle)*0.1, 0.75f + sin(angle)*0.1);
	glEnd();

	//waves
	const int wavesCount = 100;
	float blueOffsets[wavesCount];
	float yOffsets[wavesCount];
	float xOffsets[wavesCount];

	for (int i = 0; i < wavesCount; i++) {
		blueOffsets[i] = ((double)i / wavesCount) * 0.5;
		yOffsets[i] = ((double)i / wavesCount) * 0.1 + 0.9;
		xOffsets[i] = ((double)i / wavesCount) - 1.0;
	}

	for (GLint i = 0; i < wavesCount; i++) {
		GLfloat blueOffset = blueOffsets[i];
		GLfloat yOffset = yOffsets[i];
		GLfloat xOffset = xOffsets[i];
		glColor3f(blueOffset, blueOffset, 1.0);
		glLineWidth(10);
		glBegin(GL_LINE_STRIP);
		for (GLfloat x = xOffset, y = -4.0; x < 1.1; x += 0.008, y += 0.08) {
			glVertex2f(x, (sin(y) + yOffset)*0.05);
		}
		glEnd();
	}

	//boat
	glColor3f(0.51, 0.39, 0.2);
	glBegin(GL_POLYGON);
	glVertex2f(0.205, 0.11);
	glVertex2f(0.355, 0.1075);
	glVertex2f(0.45, 0.17);
	glVertex2f(0.095, 0.18);
	glEnd();

	glColor3f(1.00, 0.7, 0.7);
	glBegin(GL_POLYGON);
	glVertex2f(0.175, 0.195);
	glVertex2f(0.29, 0.645);
	glVertex2f(0.2725, 0.1975);
	glEnd();

	glColor3f(1.0, 0.6, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(0.3075, 0.7325);
	glVertex2f(0.285, 0.205);
	glVertex2f(0.3975, 0.2025);
	glEnd();

	//name and title
	glColor3f(0.0, 0.0, 0.8);
	drawText(0.8, 0.95, "Eldar Zilberman", GLUT_BITMAP_TIMES_ROMAN_10);
	glColor3f(0.3, 0.3, 0.3);
	drawText(0.77f, 0.92f, "Boat in the ocean", GLUT_BITMAP_HELVETICA_10);

	//exit button
	glColor3f(0.9, 0.9, 0.9);
	glRectf(0.89, 0.0, 1.0, 0.05);
	glColor3f(0.0, 0.0, 0.0);
	drawText(0.92f, 0.01f, "EXIT", GLUT_BITMAP_TIMES_ROMAN_10);

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Boat in the Ocean - Eldar Zilberman");

	glClearColor(0.75f, 1.0f, 1.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(drawScene);
	glutMouseFunc(handleMouse);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}