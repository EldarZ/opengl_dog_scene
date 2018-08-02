#pragma once
#include "Dog.h"
#include "Floor.h"
#include "Light.h"
#include "Table.h"

class Context {
public:
	Floor floor;
	Dog dog;
	Light light;
	Table table;
};