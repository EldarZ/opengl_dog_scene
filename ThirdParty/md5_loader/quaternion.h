#ifndef _TGQUATERNION_H
#define _TGQUATERNION_H

#include "vector3.h"
#include "defs.h"

/** Implementation des Quaternions
* Utile pour la rotation
*/

class Quaternion
{
 public:
 	inline Quaternion () : r(1), x(0), y(0), z(0) {} //par defaut, on met le quaternion identite
	inline Quaternion (float f, float i, float j, float k) : r(f), x(i), y(j), z(k) {}
	inline Quaternion (const Quaternion& q) : r(q.r), x(q.x), y(q.y), z(q.z) {}
	inline Quaternion (float f, Vector3 v) { SetRotation(f, v);}
	
	Quaternion operator = (const Quaternion& q);
	Quaternion operator + (const Quaternion& q);
	Quaternion operator - (const Quaternion& q);
	Quaternion operator * (Quaternion q) const;
	Quaternion operator / (Quaternion q);
	Quaternion operator += (const Quaternion& q);
	Quaternion operator -= (const Quaternion& q);
	Quaternion operator *= (Quaternion q);
	Quaternion operator /= (Quaternion q);
	
	Quaternion operator * (float f);
	Quaternion operator *= (float f);

	inline void Null() { r=1;x=y=z=0; }
	float Norm ();
	float Magnitude ();
	
	Vector3 GetXAxis() const;
	Vector3 GetYAxis() const;
	Vector3 GetZAxis() const;
	
	/**
	* Trouve la valeure de R, utilise par les mesh au format md5
	*/
	inline void ComputeR () { float t = 1.0f-(x*x)-(y*y)-(z*z); r = (t < 0.0f) ? 0.0f : -(float)sqrt(t); }
	
	/**
	* Normalise le quaternion
	*/
	void Normalize ();

	void GetMatrix (float*) const;
	
	Quaternion Inverse ();
	Quaternion Conjugate ();
	Quaternion Unit ();
	/**
	* Fonction calculant l'axe de rotation du Quaternion
	* @return l'axe de rotation
	*/
	Vector3 GetRotationAxis ();
	
	/**
	* Fonction calculant l'angle de rotation (radians) autour de l'axe
	* @return l'angle de rotation (radians)
	*/
	float GetRotationAngle ();

	void FromEuler (float, float, float);
	
	/**
	* Applique le Quaternion (la rotation) sur le vecteur passe en parametre et retourne le resultat
	* @param Vector3 Le vecteur sur lequel appliquer la rotation (ce vecteur n'est _pas_ modifie)
	* @return Vector3 Le resultat de la rotation sur le vecteur
	*/
	Vector3 Rotate (const Vector3& v) const;
	
	friend inline std::ostream& operator << (std::ostream& output, const Quaternion& q)
	{
		output << "[" << q.r << ", " << "(" << q.x << ", " << q.y << ", " << q.z << ")]";
		return output; 
	}
	
	float r;
	float x;
	float y;
	float z;
	
	/**
	* Redefinit la rotation du quaternion suivant l'axe et l'angle donnes
	* @param float l'angle de rotation autour de l'axe exprime en radian
	* @param Vector3 l'axe de rotation
	*/
	void SetRotation (float, const Vector3&);
	
	/**
	* Redefinit la rotation du quaternion suivant l'axe et l'angle donnes
	* @param float l'angle de rotation autour de l'axe exprime en radian
	* @param float la composante x de l'axe
	* @param float la composante y de l'axe
	* @param float la composante z de l'axe
	*/
	void SetRotation (float, float, float, float);
	
	static const Quaternion UNIT;
};


#endif

