#include "Floor.h"

Floor::Floor() : startX(-5), startZ(-5), w(5), h(5)
{
	color1[0] = 0.1f; color1[1] = 0.1f;	color1[2] = 0.1f;
	color2[0] = 0.7f; color2[1] = 0.7f;	color2[2] = 0.7f;
};

void Floor::draw()
{
	glPushMatrix();
	glBegin(GL_QUADS);

	GLfloat floor_ambient[] = { 0.02f, 0.02f, 0.02f },
		floor_diffuse[] = { 0.02f, 0.01f, 0.01f },
		floor_specular[] = { 0.4f, 0.4f, 0.4f },
		floor_shininess = .78125f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, floor_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, floor_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, floor_shininess);

	for (int x = startX; x < w; x++) {
		for (int z = startZ; z < h; z++) {
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (x + z) % 2 == 0 ? color1 : color2);
			glVertex3d(x, 0, z);
			glVertex3d(x + 1, 0, z);
			glVertex3d(x + 1, 0, z + 1);
			glVertex3d(x, 0, z + 1);
		}
	}

	glEnd();
	glPopMatrix();
}


Floor::~Floor()
{
}
