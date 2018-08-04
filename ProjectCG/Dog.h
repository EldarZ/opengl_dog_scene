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
	GLfloat tailVerticalAngle = -10.0f;
	GLfloat tailContinuesSideAngle = 0.0f;
	bool tailContinuesSideDirectionLeft = true;
	GLfloat legsAngle = 0.0f;
	bool legsAngleDirection = true;
	std::function<void()> nextMove;
	bool isMoving = false;

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
private:
	void updateConstantMovement() {
		if (tailContinuesSideAngle > 7 || tailContinuesSideAngle < -7)
		{
			tailContinuesSideDirectionLeft = !tailContinuesSideDirectionLeft;
		}
		if (tailContinuesSideDirectionLeft)
		{
			tailContinuesSideAngle += 2.0;
		}
		else {
			tailContinuesSideAngle -= 2.0;
		}
		if (isMoving) {
			if (legsAngle > 20 || legsAngle < -20)
			{
				legsAngleDirection = !legsAngleDirection;
			}
			if (legsAngleDirection)
			{
				legsAngle += 5.0;
			}
			else {
				legsAngle -= 5.0;
			}
			isMoving = false;
		}
	}
};

