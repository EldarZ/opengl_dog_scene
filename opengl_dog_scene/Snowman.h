#pragma once
#include <GL/freeglut.h>

/*
Snowman object, static rendering object
*/
class Snowman {
public:
	Snowman() = default;
	void draw();
	~Snowman() = default;
};
