#pragma once
#include <GL\freeglut.h>

/*
An Art piece, example of texture mapping of the mona lisa
*/
class Art {
public:
	Art();
	void init();
	void draw();
	~Art();
private:
	unsigned char * imageData;
	GLuint texName;
	GLsizei width;
	GLsizei height;

	unsigned char* allocateBmp(char* filename);
};