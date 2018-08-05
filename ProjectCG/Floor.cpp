#include "Floor.h"

Floor::Floor() : startX(-5), startZ(-5), w(5), h(5)
{
	color1[0] = 0.1f; color1[1] = 0.1f;	color1[2] = 0.1f;
	color2[0] = 0.7f; color2[1] = 0.7f;	color2[2] = 0.7f;
};

/*
renders the floor
*/
void Floor::draw()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);
	GLfloat floor_ambient[] = { 0.5f, 0.5f, 0.5f },
		floor_diffuse[] = { 0.9f, 0.9f, 0.9f },
		floor_specular[] = { 1.0f, 1.0f, 1.0f },
		floor_shininess = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floor_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floor_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, floor_shininess * 128.0);

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
