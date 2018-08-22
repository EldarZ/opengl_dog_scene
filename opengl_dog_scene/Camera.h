#pragma once
#include <GL\freeglut.h>

/*
the Camera controling the external view point.
*/
class Camera
{
public:
	Camera();
	GLfloat position[3];
	GLfloat target[3];
	~Camera() = default;
};

