#pragma once
#include "Dog.h"
#include "Floor.h"
#include "PointLight.h"
#include "Spotlight.h"
#include "Table.h"
#include "Teapot.h"
#include "Snowman.h"
#include "Camera.h"
#include "Art.h"
#include "Mirror.h"
#include "Walls.h"

/*
The context holds all rendered objects, a single point to access the scene objects.
*/
class Context {
public:
	GLfloat globalAmbient = 0.3f;
	int isDogView = 0;
	Camera camera;
	Floor floor;
	Dog dog;
	Table table;
	PointLight pointlight;
	SpotLight spotlight;
	Teapot teapot;
	Snowman snowman;
	Art art;
	Mirror mirror;
	Walls walls;
};