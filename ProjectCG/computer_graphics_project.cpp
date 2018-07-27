#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>
using namespace std;


class Camera
{
public:
	Camera() :x(0), y(5), z(10) {};
	double x,y,z;
	const double r = 10;
	double alpha = 0;
};

Camera gCamera;

class Floor
{
public:
	double startX, startZ, w, h;
	GLfloat color1[3]{ 0.1f,0.1f,0.1f };
	GLfloat color2[3]{ 0.7f,0.7f,0.7f };
	Floor() : startX(-5), startZ(-5), w(5), h(5) {};

	void draw()
	{
		glBegin(GL_QUADS);

		for (int x = startX; x < w; x++) {
			for (int z = startZ; z < h; z++) {
				if (z % 2 == 0)
				{
					if (x % 2 == 0) {
						glColor3fv(color1);
					}
					else {
						glColor3fv(color2);
					}
				}
				else {
					if (x % 2 == 0) {
						glColor3fv(color2);
					}
					else {
						glColor3fv(color1);
					}
				}
				//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (x + z) % 2 == 0 ? RED : WHITE);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}

		glEnd();
	}
};

Floor gFloor;

class Dog
{
public:
	void draw() {
		glPushMatrix();
		GLfloat color[3]{ 1.0f,0.0f,0.0f };
		glColor3fv(color);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(1, 1, 1);
		glutSolidSphere(1, 30, 30);

		glPushMatrix();
	}
};

Dog gDog;

// Moves the camera according to the key pressed, then ask to refresh the
// display.
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

	/*gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
		checkerboard.centerx(), 0.0, checkerboard.centerz(),
		0.0, 1.0, 0.0);*/
	/*checkerboard.draw();
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].update();
	}*/

	gluLookAt(gCamera.x, gCamera.y, gCamera.z, 0, 0, 0, 0, 1, 0);

	gFloor.draw();
	gDog.draw();


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
	glutMainLoop();
	return 0;
}