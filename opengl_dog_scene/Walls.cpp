#include "Walls.h"

Walls::Walls() :alpha(0.5f),
				color{ 1.0f,1.0f, 1.0f, -1.0f } {}

void Walls::draw(std::vector<int> wallsIndexs) {

	for (int i : wallsIndexs) {
		glPushMatrix();
		switch (i) {
		case 0:
			glRotatef(90, 1, 0, 0);
			glTranslatef(0, -5.0f, -5.0f);
			break;
		case 1:
			glRotatef(90, 0, 0, 1);
			glTranslatef(5.0f, 5.0f, 0);
			break;
		case 2:
			glRotatef(90, 0, 0, 1);
			glTranslatef(5.0f, -5.0f, 0);
			break;
		case 3:
			glRotatef(90, 1, 0, 0);
			glTranslatef(0, 5.0f, -5.0f);
			break;
		}

		glEnable(GL_BLEND);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		GLfloat ambient[] = { 0.5f, 0.5f, 0.5f },
			diffuse[] = { 0.9f, 0.9f, 0.9f },
			specular[] = { 1.0f, 1.0f, 1.0f },
			shininess = 128.0f;

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

		color[3] = alpha;
		for (int x = -5; x < 5; x++) {
			for (int z = -5; z < 5; z++) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}

		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
	}
}