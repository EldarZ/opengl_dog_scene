#pragma once
#include "Dog.h"
#include "Floor.h"
#include "Light.h"
#include "Spotlight.h"
#include "Table.h"
#include "Teapot.h"
#include "Camera.h"

class Context {
public:
	Camera camera;
	Floor floor;
	Dog dog;
	Table table;
	Light light;
	Spotlight spotlight;
	Teapot teapot;
};