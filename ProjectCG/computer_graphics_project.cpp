#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>
using namespace std;


GLfloat Eldar = 3.0f;

GLfloat slate_ambient[] = { 0.02f, 0.02f, 0.02f }, 
slate_diffuse[] = { 0.02f, 0.01f, 0.01f }, 
slate_specular[] = { 0.4f, 0.4f, 0.4f }, 
slate_shininess = .78125f;

class Camera
{
public:
	Camera() :x(0), y(5), z(10), r(10), alpha(0) {};
	double x,y,z;
	double r;
	double alpha;
};

Camera gCamera;

class Floor
{
public:
	int startX, startZ, w, h;
	GLfloat color1[3];
	GLfloat color2[3];
	Floor() : startX(-5), startZ(-5), w(5), h(5) 
	{
		color1[0] = 0.1f; color1[1] = 0.1f;	color1[2] = 0.1f;
		color2[0] = 0.7f; color2[1] = 0.7f;	color2[2] = 0.7f;
	};

	void draw()
	{
		glPushMatrix();
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_AMBIENT, slate_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, slate_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, slate_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, slate_shininess);

		for (int x = startX; x < w; x++) {
			for (int z = startZ; z < h; z++) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (x + z) % 2 == 0 ? color1 : color2);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}

		glEnd();
		glPopMatrix();
	}
};

Floor gFloor;

class Dog
{
public:
	void draw() {
		glPushMatrix();
		GLfloat color[3] = { 1.0f,0.0f,0.0f };
		glColor3fv(color);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, slate_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, slate_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, slate_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, slate_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

		glPushMatrix();
		glTranslated(0, 1, 0);
		glScalef(1, 1, 2);
		glutSolidCube(1);
		glPopMatrix();


		glPopMatrix();
	}
};

Dog gDog;

class Lamp
{
public:
	GLfloat ambient[3] = { 0.1f, 0.1f, 0.1f };
	GLfloat color[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[4] = { 0.0, 3.0,3.0 , 0.0};

	void init() 
	{
		glEnable(GL_LIGHT0);
	}

	void draw() {
		position[1] = Eldar;

		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		
		glColor3fv(color);
		GLfloat ambient[] = {10.0, 10.0, 10.0, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glTranslatef(position[0], position[1], position[2]);
		glutSolidSphere(0.2, 100, 100);

		glPopMatrix();
	}
};
Lamp gLamp;

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
		Eldar += 0.3;
		break;
	case GLUT_KEY_F2:
		Eldar -= 0.3;
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

	/*gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
		checkerboard.centerx(), 0.0, checkerboard.centerz(),
		0.0, 1.0, 0.0);*/
	/*checkerboard.draw();
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].update();
	}*/

	gluLookAt(gCamera.x, gCamera.y, gCamera.z, 0, 0, 0, 0, 1, 0);
	
	gLamp.draw();
	
	gDog.draw();
	gFloor.draw();
	

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
	gLamp.init();
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}