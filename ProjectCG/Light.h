#pragma once
#include <GL\freeglut.h>

class Light
{
private:
	GLfloat &_temp;
	GLfloat ambient[3] = { 0.1f, 0.1f, 0.1f };
	GLfloat color[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
	GLfloat position[4] = { -3.0, 3.0,3.0 , 0.0 };

public:	
	Light(GLfloat & temp) :_temp(temp) {}
	void init()
	{
		glEnable(GL_LIGHT0);
	}

	void draw();
	~Light() {};
};

