/*//Author : Eldar Zilberman

#include <windows.h>
#include <iostream>
#include <GL\freeglut.h>
using namespace std;

static int WINDOW_WIDTH = 400;
static int WINDOW_HEIGHT = 400;
static float PI = 3.1415926535;

void handleMouse(int button, int state,	int x, int y) {
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
	glViewport(0, h-newScale, newScale, newScale);
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
	const int wavesCount =100;
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
*/
// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.
/*
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

// A camera.  It moves horizontally in a circle centered at the origin of
// radius 10.  It moves vertically straight up and down.
class Camera {
	double theta;      // determines the x and z positions
	double y;          // the current y position
	double dTheta;     // increment in theta for swinging the camera around
	double dy;         // increment in y for moving the camera up/down
public:
	Camera() : theta(0), y(3), dTheta(0.04), dy(0.2) {}
	double getX() { return 10 * cos(theta); }
	double getY() { return y; }
	double getZ() { return 10 * sin(theta); }
	void moveRight() { theta += dTheta; }
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};

// A ball.  A ball has a radius, a color, and bounces up and down between
// a maximum height and the xz plane.  Therefore its x and z coordinates
// are fixed.  It uses a lame bouncing algorithm, simply moving up or
// down by 0.05 units at each frame.
class Ball {
	double radius;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;
public:
	Ball(double r, GLfloat* c, double h, double x, double z) :
		radius(r), color(c), maximumHeight(h), direction(-1),
		y(h), x(x), z(z) {
	}
	void update() {
		y += direction * 0.05;
		if (y > maximumHeight) {
			y = maximumHeight; direction = -1;
		}
		else if (y < radius) {
			y = radius; direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};

// A checkerboard class.  A checkerboard has alternating red and white
// squares.  The number of squares is set in the constructor.  Each square
// is 1 x 1.  One corner of the board is (0, 0) and the board stretches out
// along positive x and positive z.  It rests on the xz plane.  I put a
// spotlight at (4, 3, 7).
class Checkerboard {
	int displayListId;
	int width;
	int depth;
public:
	Checkerboard(int width, int depth) : width(width), depth(depth) {}
	double centerx() { return width / 2; }
	double centerz() { return depth / 2; }
	void create() {
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		GLfloat lightPosition[] = { 4, 3, 7, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
					(x + z) % 2 == 0 ? RED : WHITE);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};

// Global variables: a camera, a checkerboard and some balls.
Checkerboard checkerboard(8, 8);
Camera camera;
Ball balls[] = {
	Ball(1, GREEN, 7, 6, 1),
	Ball(1.5, MAGENTA, 6, 3, 4),
	Ball(0.4, WHITE, 5, 1, 7)
};


// Application-specific initialization: Set up global lighting parameters
// and create display lists.
void init() {
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	checkerboard.create();
}

// Draws one frame, the checkerboard then the balls, from the current camera
// position.
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
		checkerboard.centerx(), 0.0, checkerboard.centerz(),
		0.0, 1.0, 0.0);
	checkerboard.draw();
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].update();
	}
	glFlush();
	glutSwapBuffers();
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: camera.moveLeft(); break;
	case GLUT_KEY_RIGHT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowPosition(80, 80);
	//glutInitWindowSize(800, 600);
	glutCreateWindow("Bouncing Balls");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	init();
	glutMainLoop();
}*/

// This program is from the OpenGL Programming Guide.  It shows a robot arm
// that you can rotate by pressing the arrow keys.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// The robot arm is specified by (1) the angle that the upper arm makes
// relative to the x-axis, called shoulderAngle, and (2) the angle that the
// lower arm makes relative to the upper arm, called elbowAngle.  These angles
// are adjusted in 5 degree increments by a keyboard callback.
static int shoulderAngle = 0, elbowAngle = 0;

// Handles the keyboard event: the left and right arrows bend the elbow, the
// up and down keys bend the shoulder.
void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: (elbowAngle += 5) %= 360; break;
	case GLUT_KEY_RIGHT: (elbowAngle -= 5) %= 360; break;
	case GLUT_KEY_UP: (shoulderAngle += 5) %= 360; break;
	case GLUT_KEY_DOWN: (shoulderAngle -= 5) %= 360; break;
	default: return;
	}
	glutPostRedisplay();
}

// wireBox(w, h, d) makes a wireframe box with width w, height h and
// depth d centered at the origin.  It uses the GLUT wire cube function.
// The calls to glPushMatrix and glPopMatrix are essential here; they enable
// this function to be called from just about anywhere and guarantee that
// the glScalef call does not pollute code that follows a call to myWireBox.
void wireBox(GLdouble width, GLdouble height, GLdouble depth) {
	glPushMatrix();
	glScalef(width, height, depth);
	glutWireCube(1.0);
	glPopMatrix();
}

// Displays the arm in its current position and orientation.  The whole
// function is bracketed by glPushMatrix and glPopMatrix calls because every
// time we call it we are in an "environment" in which a gluLookAt is in
// effect.  (Note that in particular, replacing glPushMatrix with
// glLoadIdentity makes you lose the camera setting from gluLookAt).
void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Draw the upper arm, rotated shoulder degrees about the z-axis.  Note that
	// the thing about glutWireBox is that normally its origin is in the middle
	// of the box, but we want the "origin" of our box to be at the left end of
	// the box, so it needs to first be shifted 1 unit in the x direction, then
	// rotated.
	glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	wireBox(2.0, 0.4, 1.0);

	// Now we are ready to draw the lower arm.  Since the lower arm is attached
	// to the upper arm we put the code here so that all rotations we do are
	// relative to the rotation that we already made above to orient the upper
	// arm.  So, we want to rotate elbow degrees about the z-axis.  But, like
	// before, the anchor point for the rotation is at the end of the box, so
	// we translate <1,0,0> before rotating.  But after rotating we have to
	// position the lower arm at the end of the upper arm, so we have to
	// translate it <1,0,0> again.
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	wireBox(2.0, 0.4, 1.0);

	glPopMatrix();
	glFlush();
}

// Handles the reshape event by setting the viewport so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ratio.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 20.0);
}

// Perfroms application specific initialization: turn off smooth shading,
// sets the viewing transformation once and for all.  In this application we
// won't be moving the camera at all, so it makes sense to do this.
void init() {
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 2, 8, 0, 0, 0, 0, 1, 0);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Robot Arm");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	init();
	glutMainLoop();
}