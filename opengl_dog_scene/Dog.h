#pragma once
#include <GL\freeglut.h>
#include <functional>

/*
The Dog object, renders the dog and exposes the dog controls to the ui.
*/
class Dog
{
public:
	Dog();
	GLfloat local[16];	//local coordinate system transformation matrix
	GLfloat headHorizontalAngle; //head horizontal controled angle
	GLfloat headVerticalAngle; //head vertircal controled angle
	GLfloat tailHorizontalAngle; //tail horizontal controled angle
	GLfloat tailVerticalAngle; //tail vertical angle
	std::function<void()> nextMove; //next move function
	bool isMoving; //movement indication

	void init();
	void draw();
	~Dog() = default;
private:
	//update constant animation for tail wiggle and legs movement
	void updateConstantMovement();
	GLfloat tailWiggleAngle; //tail wiggle angle
	bool tailWiggleDirectionLeft; //tail wiggle direction toggle
	GLfloat legsAngle; //legs movement angle
	bool legsMovementDirectionForward; //legs movement direction toggle
};

