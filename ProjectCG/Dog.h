#pragma once
#include <GL\freeglut.h>

class Dog
{
public:
	GLfloat headSideRotation;
	GLfloat headVerticalRotation;
	GLfloat tailSideRotation;
	GLfloat tailVerticalRotation;

	Dog() : headSideRotation(0.0f), headVerticalRotation(10.0f), 
		tailSideRotation(0.0f), tailVerticalRotation(10.0f) {};
	void draw();
	~Dog() {}
};

