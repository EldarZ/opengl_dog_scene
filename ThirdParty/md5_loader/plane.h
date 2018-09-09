#ifndef _TGPLANE_H
#define _TGPLANE_H

#include "vector3.h"

class Plane
{
 public:
	Plane () : vNormal(0,0,0), fD(0.0f)
	{}
	Plane (Vector3 v, float f) : vNormal(v), fD(f)
	{}
	
	void SetNormal (Vector3 v) { vNormal = v; }
	void SetD (float f) { fD = f; }
	Vector3 GetNormal () { return vNormal; }
	float GetD () { return fD; }
	
	void SetFromPoints (const Vector3& v1, const Vector3& v2, const Vector3& v3);
	
	float GetDistance (const Vector3& v);
	int ClassifyPoint (const Vector3& v);
	
	bool IntersectRay (const Vector3& R0, const Vector3& end, Vector3& cP);
	
	void Normalize ();
	
	//DEBUG
	Vector3 v[3];
	Vector3 vNormal;
	//vNormal.x*x + vNormal.y*y + vNormal.z+z + fD = 0
	float fD;
};

#endif
