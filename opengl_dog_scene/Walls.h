#pragma once
#include <vector>
#include <GL\freeglut.h>

class Walls {
public:
	GLfloat alpha;
	GLfloat color[4];

	Walls();
	void draw(std::vector<int> wallsIndexs);
	~Walls() = default;
};
