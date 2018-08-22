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
	GLfloat headHorizontalAngle;
	GLfloat headVerticalAngle;
	GLfloat tailHorizontalAngle;
	GLfloat tailVerticalAngle;
	std::function<void()> nextMove;
	bool isMoving;

	void init();
	void draw();
	~Dog() = default;
private:
	//update constant animation for tail wiggle and legs movement
	void updateConstantMovement();
	GLfloat tailWiggleAngle;
	bool tailWiggleDirectionLeft;
	GLfloat legsAngle;
	bool legsMovementDirectionForward;
};

