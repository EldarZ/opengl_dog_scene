#pragma once
#include <GL\freeglut.h>

class Floor
{
private:
	int startX, startZ, w, h;
	GLfloat color1[3];
	GLfloat color2[3];
public:
	Floor();
	void draw();
	~Floor();
};
