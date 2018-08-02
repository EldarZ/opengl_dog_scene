#pragma once
class Camera
{
public:
	double x, y, z;
	double r;
	double alpha;
	Camera() :x(0), y(5), z(10), r(10), alpha(0) {};
	~Camera() {}
};

