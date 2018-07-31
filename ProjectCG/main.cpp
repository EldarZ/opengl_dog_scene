#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>

#include "Dog.h"
#include "Floor.h"
#include "Light.h"

using namespace std;

GLfloat temp = 3.0f;

class Vector
{
public:
	float x, y, z;
	Vector(float X = 0.0, float Y = 0.0, float Z = 0.0)	: x(X), y(Y), z(Z){}
	static Vector Cross(const Vector& vec1, const Vector& vec2)	{
		Vector vecCross;
		vecCross.x = vec1.y * vec2.z - vec1.z * vec2.y;
		vecCross.y = vec1.z * vec2.x - vec1.x * vec2.z;
		vecCross.z = vec1.x * vec2.y - vec1.y * vec2.x;
		vecCross.Normalize();
		return vecCross;
	}

	float Length() const { return sqrt(x * x + y * y + z * z); }
	void Normalize(){ 
		float Len = Length();
		x /= Len;
		y /= Len;
		z /= Len;
	}
};

class Camera
{
public:
	Camera() :x(0), y(5), z(10), r(10), alpha(0) {};
	double x,y,z;
	double r;
	double alpha;
};

Camera gCamera;
Floor gFloor;
Dog gDog;
Light gLamp(temp);

class Table{
public:
	void draw() {
		glPushMatrix();
		GLfloat color[3] = { 0.651, 0.502, 0.392 };
		glColor3fv(color);

		GLfloat table_ambient[] = { 0.001f, 0.001f, 0.001f },
			table_diffuse[] = { 0.0001f, 0.0001f, 0.0001f },
			table_specular[] = { 0.1f, 0.1f, 0.1f },
			table_shininess = 1.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, table_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, table_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, table_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, table_shininess);
		glMaterialf(GL_FRONT, GL_EMISSION, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

		//scale to model with higher values
		glScalef(0.3, 0.3, 0.3);
		glTranslated(-7, 3.5, -7);

		glPushMatrix();
		glScalef(7, 0.5, 7);
		glutSolidCube(1);
		glPopMatrix();

		//torso
		glPushMatrix();
		glTranslated(-2.5, -1.75, -2);
		glScalef(1, 3.5, 1);
		glutSolidCube(1);
		glPopMatrix();

		//legs
		glPushMatrix();
		glTranslated(2.5, -1.75, -2);
		glScalef(1, 3.5, 1);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-2.5, -1.75, 2);
		glScalef(1, 3.5, 1);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(2.5, -1.75, 2);
		glScalef(1, 3.5, 1);
		glutSolidCube(1);
		glPopMatrix();

		glPopMatrix();
	}
};

Table gTable;

void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: 
		gCamera.alpha -= 0.1; 
		gCamera.x = gCamera.r * sin(gCamera.alpha); 
		gCamera.z = gCamera.r * cos(gCamera.alpha);
		break;
	case GLUT_KEY_RIGHT: 
		gCamera.alpha += 0.1; 
		gCamera.x = gCamera.r * sin(gCamera.alpha);
		gCamera.z = gCamera.r * cos(gCamera.alpha);
		break;
	case GLUT_KEY_UP: gCamera.y += 1; break;
	case GLUT_KEY_DOWN: gCamera.y -= 1; break;
	case GLUT_KEY_F1:
		temp += 0.3f;
		break;
	case GLUT_KEY_F2:
		temp -= 0.3f;
		break;
	case GLUT_KEY_F3:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(gCamera.x, gCamera.y, gCamera.z, 0, 0, 0, 0, 1, 0);
	
	gLamp.draw();
	gDog.draw();
	gFloor.draw();
	gTable.draw();

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG Project");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	gLamp.init();

	glutMainLoop();
	return 0;
}