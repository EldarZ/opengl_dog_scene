#include "PointLight.h"

PointLight::PointLight() : color{ 0.691f, 0.653f, 0.254f , 1.0f },
						   position{ 4.3f, 3.0f, 0.0f , 1.0f } {}

void PointLight::addLight() {
	if (!glIsEnabled(GL_LIGHT0))
		return;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	//The initial spot cutoff is 180, resulting in uniform light distribution.
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);
}

void PointLight::draw()
{
	if (!glIsEnabled(GL_LIGHT0))
		return;
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor4fv(color);
	glutSolidSphere(0.2, 100, 100);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void PointLight::disable()
{
	glDisable(GL_LIGHT0);
}

void PointLight::enable()
{
	glEnable(GL_LIGHT0);
}