#include "vector3.h"

void Vector3::ToAngleVectors(Vector3& forward, Vector3& right, Vector3& up)
{
	float angle;
	float sr, sp, sy, cr, cp, cy;
	/*
	 * Ces formules sont prises de Quake II (q_shared.c:AngleVectors)
	 * Il faut savoir que par rapport a q2, les axes de tg sont pas les memes :
	 * y = z;
	 * z = -y;
	 */
	//Ca devrait etre y, mais pour utiliser les formules de q2, on inverse
	angle = z*DEGTORAD;
	sy = sinf(angle);
	cy = cosf(angle);
	//ca devrait etre z, mais pour utiliser les formules de q2, on echange
	angle = y*DEGTORAD;
	sp = sinf(angle);
	cp = cosf(angle);
	angle = x*DEGTORAD;
	sr = sinf(angle);
	cr = cosf(angle);

	forward.x = cp*cy;
	forward.y = cp*sy;
	forward.z = -sp;

	right.x = cr*sp*cy+-sr*-sy;
	right.y = cr*sp*sy+-sr*cy;
	right.z = cr*cp;

	up.x = sr*sp*cy+cr*-sy;
	up.y = sr*sp*sy+cr*cy;
	up.z = sr*cp;

	/*Vector3 tmp = up;
	up = -right;
	right = tmp;*/
	
	/*forward.Set(cp*cy, cp*sy, -sp);
	up.Set(cr*sp*cy+-sr*-sy,cr*sp*sy+-sr*cy, cr*cp);
	right.Set(-1*sr*sp*cy+-1*cr*-sy,-1*sr*sp*sy+-1*cr*cy,-1*sr*cp);*/
}
	
void Vector3::Cross(const Vector3 &v1, const Vector3 &v2)
{
	x = (v1.y * v2.z) - (v1.z * v2.y);
	y = (v1.z * v2.x) - (v1.x * v2.z);
	z = (v1.x * v2.y) - (v1.y * v2.x);
}

float Vector3::Magnitude() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::Normalize() const
{
	if (this->Magnitude()<EPSILON)
	{
		return Vector3 (0,0,0);
	}
	return (*this) / (this->Magnitude());
}

const Vector3 Vector3::ZERO(0,0,0);
const Vector3 Vector3::UNIT_X(1,0,0);
const Vector3 Vector3::UNIT_Y(0,1,0);
const Vector3 Vector3::UNIT_Z(0,0,1);
const Vector3 Vector3::NEGATIVE_UNIT_X(-1,0,0);
const Vector3 Vector3::NEGATIVE_UNIT_Y(0,-1,0);
const Vector3 Vector3::NEGATIVE_UNIT_Z(0,0,-1);
const Vector3 Vector3::UNIT_SCALE(1,1,1);

