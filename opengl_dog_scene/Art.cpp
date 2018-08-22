#include <iostream>
#include "Art.h"

Art::Art() {
	imageData = allocateBmp("..\\Assets\\mona.bmp");
}

void Art::init() {
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
}
void Art::draw() {
	GLfloat color[4] = { 1.0, 1.0, 1.0 , 1.0 };
	glColor4fv(color);
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f },
		shininess = 128.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0); glVertex3f(-(float)height / (2.0f*(float)width), -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-(float)height / (2.0f*(float)width), 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f((float)height / (2.0f*(float)width), 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f((float)height / (2.0f*(float)width), -1.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

unsigned char* Art::allocateBmp(char* filename)
{
	int i;
	FILE* f = nullptr;
	errno_t status = fopen_s(&f, filename, "rb");
	if (status != 0) {
		return nullptr;
	}
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	width = *(int*)&info[18];
	height = *(int*)&info[22];

	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	for (i = 0; i < size; i += 3)
	{
		unsigned char tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	return data;
}

Art::~Art() {
	delete[] imageData;
}