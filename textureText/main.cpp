#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>

GLuint tex;
GLUquadric* sphere;

void make_tex(void)
{
	unsigned char data[256][256][3];
	for (int y = 0; y < 255; y++) {
		for (int x = 0; x < 255; x++) {
			unsigned char *p = data[y][x];
			p[0] = p[1] = p[2] = (x ^ y) & 8 ? 255 : 0;
		}
	}
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *)data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void draw(void)
{
	glClearColor(0.5, 0.5, 1.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.0);
	glRotatef(45, 0, 1, 0);

	glColor3f(0.0, 1.0, 0.0);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 1.0, 32, 16);

	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (!h)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0*w / h, 0.1, 100.0);
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	make_tex();
	sphere = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 512);

	glutCreateWindow("Test");

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	init();

	glutMainLoop();
}