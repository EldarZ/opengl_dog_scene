#pragma once
#include "Dog.h"
#include "Floor.h"
#include "Light.h"
#include "Spotlight.h"
#include "Table.h"
#include "Teapot.h"
#include "Snowman.h"
#include "Camera.h"
#include "Art.h"

class Context {
public:
	int isDogView = 0;
	Camera camera;
	Floor floor;
	Dog dog;
	Table table;
	Light light;
	Spotlight spotlight;
	Teapot teapot;
	Snowman snowman;
	Art art;
};