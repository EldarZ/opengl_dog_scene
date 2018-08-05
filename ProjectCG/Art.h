#pragma once
#include <GL\freeglut.h>

class Art {
public:
	GLuint texName;
	
#define checkImageWidth 483
#define checkImageHeight 720
	void init() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST);
		artBmp = readBMP("..\\Assets\\mona.bmp");

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 483, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, artBmp);

	}
	void draw(){
		glEnable(GL_TEXTURE_2D);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
private:
	unsigned char * artBmp;


	unsigned char* readBMP(char* filename)
	{
		int i;
		FILE* f = fopen(filename, "rb");
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

												   // extract image height and width from header
		int width = *(int*)&info[18];
		int height = *(int*)&info[22];

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