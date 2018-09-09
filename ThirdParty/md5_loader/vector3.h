#ifndef _TGVECTOR_H
#define _TGVECTOR_H
#include "defs.h"

class Vector3
{
 public:
	float x;
	float y;
	float z;
	inline Vector3 () : x(0), y(0), z(0) {};
	inline Vector3(float nx, float ny, float nz = 0) : x(nx), y(ny), z(nz) {}
	inline Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
	~Vector3 () {};
	
	void ToAngleVectors (Vector3&, Vector3&, Vector3&);

	void Cross(const Vector3 &v1, const Vector3 &v2);
	float Magnitude() const;
	Vector3 Unit() const 
	{ 
		return this->Normalize(); 
	}
	Vector3 Normalize() const;
	
	void Null () { x = y = z = 0; }
	
	inline void Set(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}
	
	friend inline std::ostream& operator << (std::ostream& output, const Vector3& v)
	{
		output << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return output; 
	}
	
	inline float operator [] (int n) {  switch (n) { case 0: return x; case 1: return y; case 2: return z;}}

	inline bool operator != (const Vector3& v) { return (v.x==x&&v.y==y&&v.z==z); }

	inline bool equals (const Vector3& v) {
		if ( ((v.x-EPSILON<x) && (v.x+EPSILON>x)) && 
		     ((v.y-EPSILON<y) && (v.y+EPSILON>y)) && 
		     ((v.z-EPSILON<z) && (v.y+EPSILON>y)))
			return true;
		return false;
	}
		
	inline friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2) 
	{
		return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	
	inline friend bool operator > (const Vector3& v1, const Vector3& v2)
	{
		return (v1.x > v2.x && v1.y > v2.y && v1.z > v2.z);
	}
	
	inline friend bool operator < (const Vector3& v1, const Vector3& v2)
	{
		return (v1.x < v2.x && v1.y < v2.y && v1.z < v2.z);
	}
	
	inline friend bool operator == (const Vector3& v1, const Vector3& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
	}
	
	//cross product
	inline friend Vector3 operator%(const Vector3 &v1, const Vector3 &v2) 
	{
		return Vector3((v1.y * v2.z) - (v1.z * v2.y),(v1.z * v2.x) - (v1.x * v2.z),(v1.x * v2.y) - (v1.y * v2.x));
	}
	
	inline friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2) 
	{
		return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}
	//dot product
	inline friend float operator*(const Vector3 &v1, const Vector3 &v2) 
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}
	
	inline friend Vector3 operator*(const Vector3 &v1, float s) 
	{
		return Vector3(v1.x * s, v1.y * s, v1.z * s);
	}
	
	inline friend Vector3 operator*(float s, const Vector3 &v1) 
	{
		return Vector3(v1.x * s, v1.y * s, v1.z * s);
	}
	
	inline friend Vector3 operator/(const Vector3 &v1, float s) 
	{
		s = 1.0f / s;
		return Vector3(v1.x * s, v1.y * s, v1.z * s);
	}
	
	inline Vector3 & operator+=(const Vector3 &v1) 
	{
		x += v1.x;
		y += v1.y;
		z += v1.z;
		return *this;
	}
	
	inline Vector3 & operator-=(const Vector3 &v1) {
		x -= v1.x;
		y -= v1.y;
		z -= v1.z;
		return *this;
	}
	
	inline Vector3 & operator*=(float s) 
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	
	inline Vector3 & operator/=(float s) 
	{
		if(s!=0)
		{
			s = 1.0f / s;
			x *= s;
			y *= s;
			z *= s;
		}
		return *this;
	}
	
	inline Vector3 operator+() const 
	{ 
		return *this; 
	}
	
	inline Vector3 operator-() const 
	{ 
		return Vector3(-x, -y, -z); 
	}
	
	
	//Vecteurs speciaux:
	static const Vector3 ZERO;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 NEGATIVE_UNIT_X;
	static const Vector3 NEGATIVE_UNIT_Y;
	static const Vector3 NEGATIVE_UNIT_Z;
	static const Vector3 UNIT_SCALE;
};

#endif
