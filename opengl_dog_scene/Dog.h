#pragma once
#include <GL\freeglut.h>
#include <functional>

/*
The Dog object, renders the dog and exposes the dog controls to the ui.
*/
class Dog
{
public:
	//local accumolated transformation matrix
	GLfloat local[16];	
	
	//head horizontal controled angle
	GLfloat headHorizontalAngle = 0.0f;
	
	//head vertircal controled angle
	GLfloat headVerticalAngle = 10.0f;
	
	//tail horizontal controled angle
	GLfloat tailHorizontalAngle = 0.0f;

	//tail vertical angle
	GLfloat tailVerticalAngle = -10.0f;

	//tail wiggle angle
	GLfloat tailWiggleAngle = 0.0f;

	//tail wiggle direction toggle
	bool tailWiggleDirectionLeft = true;

	//legs movement angle
	GLfloat legsAngle = 0.0f;
	
	//legs movement direction toggle
	bool legsDirectionAngle = true;

	//next move function
	std::function<void()> nextMove;

	//movement indication
	bool isMoving = false;

	void init() {
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadIdentity();

		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.5, 3.5f * 0.30f, -2.8f);

		glGetFloatv(GL_MODELVIEW_MATRIX, local);
		glLoadMatrixf(viewModelMatrix);
	}
	void draw();
private:
	//update constant animation for tail wiggle and legs movement
	void updateConstantMovement() {
		if (tailWiggleAngle > 8 || tailWiggleAngle < -8)
		{
			tailWiggleDirectionLeft = !tailWiggleDirectionLeft;
		}
		if (tailWiggleDirectionLeft)
		{
			tailWiggleAngle += 1.7f;
		}
		else {
			tailWiggleAngle -= 1.7f;
		}
		if (isMoving) {
			if (legsAngle > 20 || legsAngle < -20)
			{
				legsDirectionAngle = !legsDirectionAngle;
			}
			if (legsDirectionAngle)
			{
				legsAngle += 6.0;
			}
			else {
				legsAngle -= 6.0;
			}
			isMoving = false;
		}
	}
};

