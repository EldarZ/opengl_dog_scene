#pragma once
#include "GL/freeglut.h"

class Mirror {
public:
	Mirror();
	void draw();
	~Mirror() = default;
private:
	GLfloat color[4];
};