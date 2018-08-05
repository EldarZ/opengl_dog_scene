#pragma once
#include <GL\freeglut.h>

/*
An Art piece, example of texture mapping of mona lisa
*/
class Art {
public:
	Art() {
		artBmp = readBMP("..\\Assets\\mona.bmp");
	}

	void init() {
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, artBmp);
	}
	void draw(){
		GLfloat color[3] = { 1.0, 1.0, 1.0 };
		glColor3fv(color);
		GLfloat ambient[] = { 1.0f, 1.0f, 1.0f },
			diffuse[] = { 1.0f, 1.0f, 1.0f },
			specular[] = { 1.0f, 1.0f, 1.0f },
			shininess = 128.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		glMaterialf(GL_FRONT, GL_EMISSION, 100);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	~Art() {
		delete[] artBmp;
	}
private:
	unsigned char * artBmp;
	GLuint texName;
	GLsizei width;
	GLsizei height;

	unsigned char* readBMP(char* filename)
	{
		int i;
		FILE* f = fopen(filename, "rb");
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

};