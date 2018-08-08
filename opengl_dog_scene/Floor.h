#pragma once
#include <GL\freeglut.h>

/*
The floor 
*/
class Floor
{
private:
	int startX, startZ, endx, endz;

	GLfloat color1[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat color2[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
public:
	Floor();
	void draw();
	~Floor();
};

