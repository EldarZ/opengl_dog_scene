#include "Table.h"
#include <GL\freeglut.h>
#include <iostream>

GLuint tex;

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

Table::Table() {
	woodBmp = readBMP("..\\Assets\\woodTex.bmp");

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *)woodBmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Table::~Table() {
	delete[] woodBmp;
}

void Table::draw()
{
	glPushMatrix();
	GLfloat color[3] = { 0.651, 0.502, 0.392 };
	glColor3fv(color);

	GLfloat table_ambient[] = { 0.001f, 0.001f, 0.001f },
		table_diffuse[] = { 0.0001f, 0.0001f, 0.0001f },
		table_specular[] = { 0.1f, 0.1f, 0.1f },
		table_shininess = 0.3f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, table_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, table_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, table_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, table_shininess);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); 

	//scale to model with higher values
	glScalef(0.3, 0.3, 0.3);

	//top
	glPushMatrix();
	glScalef(7, 0.5, 7);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex);

	glutSolidCube(1);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();

	//legs
	glPushMatrix();
	glTranslated(-2.5, -1.75, -2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.5, -1.75, -2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, -1.75, 2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.5, -1.75, 2);
	glScalef(1, 3.5, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}