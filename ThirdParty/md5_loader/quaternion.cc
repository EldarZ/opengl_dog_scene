#include "quaternion.h"



Quaternion Quaternion::operator = (const Quaternion& q)
{
	r = q.r;
	x = q.x;
	y = q.y;
	z = q.z;
	return (*this);
}

Quaternion Quaternion::operator + (const Quaternion& q)
{
	return Quaternion(r+q.r, x+q.x, y+q.y, z+q.z);
}

Quaternion Quaternion::operator - (const Quaternion& q)
{
	return Quaternion(r-q.r, x-q.x, y-q.y, z-q.z);
}

Quaternion Quaternion::operator * (Quaternion q) const
{
	return Quaternion (r*q.r - x*q.x - y*q.y - z*q.z, 
			   r*q.x + x*q.r + y*q.z - z*q.y,
			   r*q.y + y*q.r + z*q.x - x*q.z,
			   r*q.z + z*q.r + x*q.y - y*q.x);
}

Quaternion Quaternion::operator / (Quaternion q)
{
	return (*this) * q.Inverse();
}

Quaternion Quaternion::operator += (const Quaternion& q)
{
	r += q.r;
	x += q.x;
	y += q.y;
	z += q.z;
	return (*this);
}

Quaternion Quaternion::operator -= (const Quaternion& q)
{
	r -= q.r;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return (*this);
}

Quaternion Quaternion::operator *= (Quaternion q)
{
	(*this) = q * (*this);
	return (*this);
}

Quaternion Quaternion::operator /= (Quaternion q)
{
	(*this) = (*this)*q.Inverse();
  	return (*this);
}

Quaternion Quaternion::operator * (float f)
{
	return Quaternion (r*f, x*f, y*f, z*f);
}

Quaternion Quaternion::operator *= (float f)
{
	(*this) = (*this)*f;
	return (*this);
}

Vector3 Quaternion::GetXAxis () const
{
	//float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twy = ty*r;
	float twz = tz*r;
	float txy = ty*x;
	float txz = tz*x;
	float tyy = ty*y;
	float tzz = tz*z;

	return Vector3 (1.0f-(tyy+tzz), txy+twz, txz-twy);
}

Vector3 Quaternion::GetYAxis () const
{
	float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twx = tx*r;
	float twz = tz*r;
	float txx = tx*x;
	float txy = ty*x;
	float tyz = tz*y;
	float tzz = tz*z;

	return Vector3(txy-twz, 1.0f-(txx+tzz), tyz+twx);
}

Vector3 Quaternion::GetZAxis () const
{
	float tx = 2*x;
	float ty = 2*y;
	float tz = 2*z;
	float twx = tx*r;
	float twy = ty*r;
	float txx = tx*x;
	float txz = tz*x;
	float tyy = ty*y;
	float tyz = tz*y;

	return Vector3(txz+twy, tyz-twx, 1.0f-(txx+tyy));
}

void Quaternion::FromEuler(float x, float y, float z)
{
	Quaternion qx (x, Vector3(1,0,0));
	Quaternion qy (y, Vector3(0,1,0));
	Quaternion qz (z, Vector3(0,0,1));
	Quaternion qt;
	
	qt = qx*qy;
	*this = qt*qz;
}


float Quaternion::Norm()
{
	return (r*r + x*x + y*y + z*z);
}

void Quaternion::Normalize()
{
	float mag = this->Magnitude();
	r /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
}

float Quaternion::Magnitude()
{
	return sqrt(Norm());
}

Quaternion Quaternion::Inverse ()
{
	return Conjugate()*(1/Norm());
}

Quaternion Quaternion::Conjugate ()
{
	return Quaternion (r, -x, -y, -z);
}

Quaternion Quaternion::Unit ()
{
	return (*this)*(1/(this->Magnitude()));
}

Vector3 Quaternion::Rotate (const Vector3& v) const
{
	Quaternion vec(0, v.x, v.y, v.z);
	Quaternion q = *this;
	Quaternion qinv = q.Conjugate();

	Quaternion vec2 = q*vec*qinv;

	return Vector3 (vec2.x, vec2.y, vec2.z);
	//nVidia SDK
	// trouve dans Ogre3d
	/*Vector3 uv, uuv;
	Vector3 qvec(x,y,z);
	uv = qvec%v;
	uuv = qvec%uv;
	uv *= (2.0f*r);
	uuv *= 2.0f;
	return v + uv + uuv;*/
	/*Quaternion qv(0, v.x, v.y, v.z);
	Quaternion qm = (*this)*qv*(this->Inverse());
	return Vector3 (qm.x, qm.y, qm.z);*/
}

void Quaternion::GetMatrix(float* m) const
{
	m[0]  = 1.0f - 2.0f * (y * y + z * z);
	m[1]  = 2.0f * (x * y + z * r);
	m[2]  = 2.0f * (x * z - y * r);
	m[3]  = 0.0f;
	 
	m[4]  = 2.0f * (x * y - z * r);
	m[5]  = 1.0f - 2.0f * (x * x + z * z);
	m[6]  = 2.0f * (z * y + x * r);
	m[7]  = 0.0f;

	m[8]  = 2.0f * (x * z + y * r);
	m[9]  = 2.0f * (y * z - x * r);
	m[10] = 1.0f - 2.0f * (x * x + y * y);
	m[11] = 0.0f;

	m[12] = 0; 
	m[13] = 0;  
	m[14] = 0;
	m[15] = 1.0f;
}

void Quaternion::SetRotation (float a,const Vector3& tv)
{
	Vector3 tmp = tv.Normalize ();
	a *= DEGTORAD;
	float sin_a = sinf (a/2);
	float cos_a = cosf (a/2);
	x = tmp.x*sin_a;
	y = tmp.y*sin_a;
	z = tmp.z*sin_a;
	r = cos_a;
}

void Quaternion::SetRotation (float a, float i, float j, float k)
{
	SetRotation (a, Vector3(i,j,k));
}

Vector3 Quaternion::GetRotationAxis()
{
	this->Normalize();
	float cos_a = r;
	float sin_a = sqrt (1.0 - cos_a*cos_a);
	if (fabs(sin_a) < EPSILON) sin_a = 1;
	return Vector3 (x/sin_a, y/sin_a, z/sin_a);
}

float Quaternion::GetRotationAngle()
{
	float cos_a = r;
	return acos(cos_a)*2;
}

const Quaternion Quaternion::UNIT(1,0,0,0);


