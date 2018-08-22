#pragma once
#include <GL\freeglut.h>

/*
The floor 
*/
class Floor
{
public:
	Floor();
	void draw();
	~Floor() = default;
private:
	int startX, startZ, endx, endz;
	GLfloat color1[4];
	GLfloat color2[4];
};

