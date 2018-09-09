#ifndef _MD5MESH
#define _MD5MESH

#include "vector3.h"
#include "quaternion.h"
#include "material.h"
#include "plane.h"

struct Face
{
	unsigned int pIndex[3];
	Plane plane;
	bool visible;
};

struct Vertice
{
	Vector3 vPosition;
	float s, t;
	Vector3 sTangent, tTangent;
	Vector3 vNormal;
	Vector3 vTangentSpaceLight;
};

class MD5Mesh
{
 protected:
	struct _MD5Joint;
	struct _MD5Mesh;
	struct _MD5Vert;
	struct _MD5Weight;
	struct _MD5Triangle;
	struct _MD5Anim;
 public:
	MD5Mesh ();
	~MD5Mesh ();
	bool LoadMesh(char* str);
	bool LoadAnim(char* str);
	
	void SetAnimation (const std::string& name);
	void PreRender (float);
	
	inline Material* GetMaterial (int imesh) { return pMeshes[imesh].pMaterial; }
	inline unsigned int* GetIndexes(int i) { return pMeshes[i].pIndexes; }
	
	unsigned int GetMeshesCount () { return iNumMeshes; }
	unsigned int GetTrianglesCount (int imesh=0) { return pMeshes[imesh].iNumTris; }
	Vertice* GetVertexList (int imesh=0) { return pVerticesListTab[imesh]; }
	
 protected:
	void _BuildBone (int, _MD5Joint*, const Quaternion&, const Vector3&);
	void _SkinMesh ();
	void _CalculateBBox ();
 
	_MD5Joint* pJoints;
	_MD5Joint** pBaseJoints;
	
	Vertice**  pVerticesListTab;
	Material** pMaterialTab;
	
	//animation
	float fPreviousTime;
	float fCurrentTime;
	int iFrame;
	int iCurrentAnimation;
	
	
	int iNumJoints;
	int iNumBaseJoints;
	int iNumMeshes;
	
	int** pIndexes;
 
	_MD5Mesh* pMeshes;

	//typedef std::map<std::string, _MD5Anim> AnimationList;
	//AnimationList mAnimations;
	std::vector<_MD5Anim> pAnimations;
	
	/*struct _MD5Attribute
	{
		union
		{
			struct
			{
				int X:1;
				int Y:1;
				int Z:1;
				int QX:1;
				int QY:1;
				int QZ:1;
			};
			int flag;
		};
	};*/
	
	struct _MD5Anim
	{
		std::string sName;
		int iNumFrames;
		int iNumJoints;
		int iFrameRate;
		int iNumAnimatedComponents;
		float** pFrames; //un tableau de [iNumFrames][iNumAnimatedComponents]
		float** pBaseFrame; //un tableau de [iNumJoints][6], stockant, dans l'ordre, Tx, Ty, Tz, Qx, Qy, Qz a la frame de base de l'animation
		struct _MD5JointInfos
		{
			int iParent;
			//_MD5Attribute iFlags; //les flags indiquant quelles composantes du bone sont animees
			int iFlags;
			int iStartIndex;
		};
		_MD5JointInfos* pJointInfos; //un tableau de iNumJoints
	};

	
	
	struct _MD5Joint
	{
		std::string sName;
		int iNumber;
		int iParent;
		Vector3 vPosition;
		//Matrix33 mOrientation;
		//float mOrientation[9];
		Quaternion qOrientation;
		
		_MD5Joint* pParent;
		_MD5Joint** pChildrens;
		int iNumChildrens;
	};
	
	struct _MD5Mesh
	{
		int iNumVerts;
		_MD5Vert* pVerts;
		
		int iNumWeights;
		_MD5Weight* pWeights;
		
		int iNumTris;
		_MD5Triangle* pTriangles;
		
		unsigned int* pIndexes;
		
		Material* pMaterial;
		std::string sColor;
		std::string sNormal;
	};
	
	struct _MD5Vert
	{
		int iWeightIndex;
		int iNumWeights;
		float pTexCoords[2];
	};
	
	struct _MD5Weight
	{
		int iBoneIndex;
		float fBias;
		Vector3 vWeights;
	};
	
	struct _MD5Triangle : public Face
	{
	
	};

	void ChangeCoords(Vector3& v)
	{
		float temp = v.y;
		v.y = v.z;
		v.z = -temp;
	}

	
};

#endif

