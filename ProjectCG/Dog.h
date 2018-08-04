#pragma once
#include <GL\freeglut.h>
#include <functional>
class Dog
{
public:
	GLfloat local[16];
	
	GLfloat headSideRotation = 0.0f;
	GLfloat headVerticalRotation = 10.0f;
	GLfloat tailSideRotation = 0.0f;
	GLfloat tailVerticalRotation = -10.0f;

	GLfloat tailContinuesSideRotation = 0.0f;
	bool tailContinuesSideDirectionLeft = true;

	std::function<void()> nextMove;
	
	Dog() {};
	
	void init() {
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadIdentity();

		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 3.5 * 0.3, -3);

		glGetFloatv(GL_MODELVIEW_MATRIX, local);
		glLoadMatrixf(viewModelMatrix);
	}
	void draw();
	~Dog() {}

private:
	void updateConstantMovement() {
		if (tailContinuesSideRotation > 7 || tailContinuesSideRotation < -7)
		{
			tailContinuesSideDirectionLeft = !tailContinuesSideDirectionLeft;
		}
		if (tailContinuesSideDirectionLeft)
		{
			tailContinuesSideRotation += 2.0;
		}
		else {
			tailContinuesSideRotation -= 2.0;
		}
	}
};

