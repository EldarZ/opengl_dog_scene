#include "md5mesh.h"
#include <string>

MD5Mesh::MD5Mesh ()
{
	fPreviousTime = 0;
	iFrame = 0;
	fCurrentTime = 0;
	iCurrentAnimation = -1;
}

MD5Mesh::~MD5Mesh ()
{
	delete [] pJoints;
	for (int i=0; i<iNumMeshes; i++)
	{
		delete [] pMeshes[i].pVerts;
		delete [] pMeshes[i].pTriangles;
		delete [] pMeshes[i].pIndexes;
		delete [] pMeshes[i].pWeights;
	}
	delete [] pMeshes;
	for (unsigned int i=0; i<pAnimations.size(); i++)
	{
		for (int j=0; j<pAnimations[i].iNumJoints; j++)
			delete [] pAnimations[i].pBaseFrame[j];
		delete [] pAnimations[i].pBaseFrame;
		delete [] pAnimations[i].pJointInfos;
		
		for (int j=0; j<pAnimations[i].iNumFrames; j++)
			delete [] pAnimations[i].pFrames[j];
		delete [] pAnimations[i].pFrames;
		
	}
	pAnimations.clear();
}


#define NEXT(str) while(buffer.find(str)==std::string::npos) \
			pFD >> buffer;

bool MD5Mesh::LoadMesh(char* str)
{
	std::ifstream pFD;
	std::string buffer;
	int version;
	
	std::cout << "[TGE] Chargement de " << str << std::endl;
	
	pFD.open(str, std::ios::in);
	
	if (pFD.fail())
	{
		std::cerr << "[TGE] Erreur lors de l'ouverture de " << str << std::endl;
		return false;
	}
	
	pFD >> buffer;
	pFD >> version;
	
	if (version != 10)
	{
		std::cerr << "[TGE] Mauvaise version : " << version << std::endl;
		return false;
	}
	
	NEXT("numJoints")
	pFD >> iNumJoints;
	
	NEXT("numMeshes")
	pFD >> iNumMeshes;
	
	iNumBaseJoints = 0;
	pJoints = new _MD5Joint[iNumJoints];
	
	for (int i=0; i<iNumJoints; i++)
	{
		NEXT("\"")
		pJoints[i].sName = buffer;
		Quaternion q;
		pFD >> pJoints[i].iParent;
		if (pJoints[i].iParent < 0)
			iNumBaseJoints++;
		NEXT("(")
		pFD >> pJoints[i].vPosition.x >> pJoints[i].vPosition.y >> pJoints[i].vPosition.z;
		
		NEXT(")")
		NEXT("(")
		pFD >> q.x >> q.y >> q.z;
		
		q.ComputeR();
		
		pJoints[i].qOrientation = q;
	}
	pMeshes = new _MD5Mesh[iNumMeshes];
	for (int i=0; i<iNumMeshes; i++)
	{
		NEXT("shader")
		std::string sTmp;
		pFD >> sTmp;
		int iLastSlash = sTmp.find_first_of("\"");
		int iLastQuote = sTmp.find_last_of("\"");
		iLastSlash += 1; //on veut le caractere juste apres le dernier slash
		sTmp.erase(iLastQuote);
		auto file = std::string("..\\Assets\\") + sTmp.substr(iLastSlash);
		pMeshes[i].sNormal = file + "_local.tga";
		pMeshes[i].sColor = file + ".tga";
		//pMeshes[i].sNormal += ".tga";
		


		pMeshes[i].pMaterial = new Material ();
		Texture* tmpTex = new Texture ();
		tmpTex->LoadTexture((char*)pMeshes[i].sNormal.data(), TG_TEXTURE_2D);
		pMeshes[i].pMaterial->SetNormalTexture(tmpTex);
		tmpTex = new Texture();
		tmpTex->LoadTexture((char*)pMeshes[i].sColor.data(), TG_TEXTURE_2D);
		pMeshes[i].pMaterial->SetFirstTexture(tmpTex);
		
		
		NEXT("numverts")
		pFD >> pMeshes[i].iNumVerts;
		pFD >> buffer; //sans ca, le NEXT("vert") ne va pas bouger car vert est deja dans numverts
		
		pMeshes[i].pVerts = new _MD5Vert[pMeshes[i].iNumVerts];
		for (int j=0; j<pMeshes[i].iNumVerts; j++)
		{
			_MD5Vert& vert = pMeshes[i].pVerts[j];
			NEXT("vert")
			NEXT("(")
			pFD >> vert.pTexCoords[0] >>vert.pTexCoords[1];
			vert.pTexCoords[1] = 1 - vert.pTexCoords[1];
			NEXT(")")
			pFD >> vert.iWeightIndex >> vert.iNumWeights;
			
		}
		NEXT("numtris")
		pFD >> pMeshes[i].iNumTris;
		
		pMeshes[i].pTriangles = new _MD5Triangle[pMeshes[i].iNumTris];
		pMeshes[i].pIndexes = new unsigned int[pMeshes[i].iNumTris*3];
		int iC = 0;
		pFD >> buffer; //sans ca, le NEXT("tri") ne va pas bouger car tri est deja dans numtris
		for (int j=0; j<pMeshes[i].iNumTris; j++)
		{
			_MD5Triangle& tri = pMeshes[i].pTriangles[j];
			NEXT("tri")
			pFD >> buffer;
			pFD >> tri.pIndex[0] >> tri.pIndex[1] >> tri.pIndex[2];
			
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[0];
			iC++;
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[1];
			iC++;
			pMeshes[i].pIndexes[iC] = pMeshes[i].pTriangles[j].pIndex[2];
			iC++;
		}
		
		NEXT("numweights")
		pFD >> pMeshes[i].iNumWeights;
		pFD >> buffer; // sans ca, le NEXT("weight") foire
		pMeshes[i].pWeights = new _MD5Weight[pMeshes[i].iNumWeights];
		for (int j=0; j<pMeshes[i].iNumWeights; j++)
		{
			_MD5Weight& w = pMeshes[i].pWeights[j];
			NEXT("weight")
			pFD >> buffer;
			pFD >> w.iBoneIndex >> w.fBias;
			NEXT("(")
			pFD >> w.vWeights.x >> w.vWeights.y >> w.vWeights.z;
			//Util::ChangeCoords(w.vWeights);
		}
	}
	
	std::cout << "[TGE] Chargement de " << str << " termine" <<  std::endl;
	
	
	iNumJoints = iNumJoints;
	iNumMeshes = iNumMeshes;
	pBaseJoints = new _MD5Joint*[iNumBaseJoints];
	
	pVerticesListTab = new Vertice*[iNumMeshes];
	for (int i=0; i<iNumMeshes; i++)
		pVerticesListTab[i] = new Vertice[pMeshes[i].iNumVerts];
		
		
	//Creation des liens parents-enfants
	//mise a zero du nombre d'enfants des joints
	int* tmpCount = new int [iNumJoints]; //juste pour savoir combien de joints enfants on deja ete ajoutes a chaque joint
	for (int i=0; i<iNumJoints; i++)
	{
		pJoints[i].iNumber = i;
		pJoints[i].iNumChildrens = 0;
		tmpCount[i] = 0;
	}

	//recherche des pointeurs sur les joints parents
	//incrementation du nombre de joint enfant sur le parent
	int bIndex = 0; //compteur secondaire pour le nombre de basejoints
	for (int i=0; i<iNumJoints; i++)
	{
		if (pJoints[i].iParent < 0)
		{
			pBaseJoints[bIndex] = &pJoints[i];
			pBaseJoints[i]->pParent = NULL;
			bIndex++;
			continue;
		}
		else
		{
			pJoints[i].pParent = &pJoints[pJoints[i].iParent];
			pJoints[i].pParent->iNumChildrens++;
		}
	}
	
	//creation du tableau contenant les pointeurs sur les joint enfant
	for (int i=0; i<iNumJoints; i++)
		pJoints[i].pChildrens = new _MD5Joint*[pJoints[i].iNumChildrens];
	
	for (int i=0; i<iNumJoints; i++)
	{
		if (pJoints[i].pParent)
		{
			pJoints[i].pParent->pChildrens[tmpCount[pJoints[i].iParent]] = &pJoints[i]; //on ajoute ce bone au tableau d'enfants du parent
			tmpCount[pJoints[i].iParent]++; //on increment tmpCount du bone parent
		}
	}
	
	_SkinMesh();
	return true;

}

bool MD5Mesh::LoadAnim(char* str)
{
	_MD5Anim anim;
	//on garde juste le nom de l'animation
	std::string sTemp = std::string(str);
	int iLastSlash = sTemp.find_last_of("\\");
	int iLastPoint = sTemp.find_last_of(".");
	sTemp.erase(iLastPoint);
	anim.sName = sTemp.substr(iLastSlash+1);
	std::cout << "[TGE] anim name : " << anim.sName << std::endl;
	
	std::ifstream pFD;
	std::string buffer;
	int version;
	
	std::cout << "[TGE] Chargement de " << str << std::endl;
	
	pFD.open(str, std::ios::in);
	
	if (pFD.fail())
	{
		std::cerr << "[TGE] Erreur lors de l'ouverture de " << str << std::endl;
		return false;
	}
	
	pFD >> buffer;
	pFD >> version;
	
	if (version != 10)
	{
		std::cerr << "[TGE] Mauvaise version : " << version << std::endl;
		return false;
	}
	
	NEXT("numFrames")
	pFD >> anim.iNumFrames;
	NEXT("numJoints")
	pFD >> anim.iNumJoints;
	NEXT("frameRate")
	pFD >> anim.iFrameRate;
	NEXT("numAnimatedComponents")
	pFD >> anim.iNumAnimatedComponents;
	
	anim.pFrames = new float*[anim.iNumFrames];
	for (int i=0; i<anim.iNumFrames; i++)
		anim.pFrames[i] = new float[anim.iNumAnimatedComponents];
		
	anim.pJointInfos = new _MD5Anim::_MD5JointInfos[anim.iNumJoints];
	
	anim.pBaseFrame = new float*[anim.iNumJoints];
	
	//FIXME: a priori, il n'y a pas de md5mesh ou la hierarchie de l'anim est different du md5mesh. Ceci dit, on peut peut-etre utiliser ca pour demembrer ou autre
	NEXT("hierarchy")
	for (int i=0; i<anim.iNumJoints; i++)
	{
		NEXT("\"")
		buffer = "";
		pFD >> anim.pJointInfos[i].iParent;
		pFD >> anim.pJointInfos[i].iFlags;
		pFD >> anim.pJointInfos[i].iStartIndex;
	}
	
	//chargement de baseframe
	NEXT("baseframe")
	for (int i=0; i<anim.iNumJoints; i++)
	{
		anim.pBaseFrame[i] = new float[6];
		NEXT("(")
		pFD >> anim.pBaseFrame[i][0];
		pFD >> anim.pBaseFrame[i][1];
		pFD >> anim.pBaseFrame[i][2];
		NEXT(")")
		NEXT("(")
		pFD >> anim.pBaseFrame[i][3];
		pFD >> anim.pBaseFrame[i][4];
		pFD >> anim.pBaseFrame[i][5];
		NEXT(")")
	}
	//chargement des frames
	for (int i=0; i<anim.iNumFrames; i++)
	{
		NEXT("frame")
		NEXT("{")
		for (int j=0; j<anim.iNumAnimatedComponents; j++)
			pFD >> anim.pFrames[i][j];
		NEXT("}")
	}
	pAnimations.push_back(anim);
	std::cout << "[TGE] Chargement de " << str << " termine" << std::endl;
	return true;
}



void MD5Mesh::PreRender(float fTime)
{
	fCurrentTime += fTime;
	if (iCurrentAnimation < 0)
		return;
	if ((fCurrentTime - fPreviousTime) > 41.66/1000) // 1/24 = 0.0.41666667. Un md5 est anime 24 fois par secondes
	{
		fPreviousTime = fCurrentTime;
		iFrame++;
		if (iFrame >= pAnimations[iCurrentAnimation].iNumFrames)
			iFrame = 0;
		for (int i=0; i<iNumBaseJoints; i++)
		{
			_BuildBone(iFrame, pBaseJoints[i], Quaternion::UNIT, Vector3::ZERO);
		}
		_SkinMesh();
	}
	
}

void MD5Mesh::_SkinMesh ()
{
	Vector3 pos;
	for (int i=0; i<iNumMeshes; i++)
	{
		for (int j=0; j<pMeshes[i].iNumVerts; j++)
		{
			pVerticesListTab[i][j].vPosition.Null();
			//assignation des texcoords.
			//FIXME: On pourrait éviter ça et ne le faire qu'une seule fois nan ?
			pVerticesListTab[i][j].s = pMeshes[i].pVerts[j].pTexCoords[0];
			pVerticesListTab[i][j].t = pMeshes[i].pVerts[j].pTexCoords[1];

			for (int k=0; k<pMeshes[i].pVerts[j].iNumWeights; k++) 
			{
				int baseIndex = pMeshes[i].pVerts[j].iWeightIndex;
				int boneIndex = pMeshes[i].pWeights[baseIndex+k].iBoneIndex;
			
				pos = pJoints[boneIndex].qOrientation.Rotate(pMeshes[i].pWeights[baseIndex+k].vWeights);
 				pVerticesListTab[i][j].vPosition += (pos + pJoints[boneIndex].vPosition) * pMeshes[i].pWeights[baseIndex+k].fBias;
			}
		}
	}
}

void MD5Mesh::_BuildBone (int iFrame, _MD5Joint* pJoint,const Quaternion& q, const Vector3& v)
{
	Vector3 animatedPosition;
	animatedPosition.x = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][0];
	animatedPosition.y = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][1];
	animatedPosition.z = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][2];
	
	Quaternion animatedOrientation;
	animatedOrientation.x = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][3];
	animatedOrientation.y = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][4];
	animatedOrientation.z = pAnimations[iCurrentAnimation].pBaseFrame[pJoint->iNumber][5];
	
	
	int flags = pAnimations[iCurrentAnimation].pJointInfos[pJoint->iNumber].iFlags;
	int n=0;
	int sIndex = pAnimations[iCurrentAnimation].pJointInfos[pJoint->iNumber].iStartIndex;

	if (flags & 1) //Tx est anime
	{
		animatedPosition.x = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}
	if (flags & 2) //Ty est anime
	{
		animatedPosition.y = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}
	if (flags & 4) //Tz est anime
	{
		animatedPosition.z = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}
	if (flags & 8) //Qx est anime
	{
		animatedOrientation.x = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}
	if (flags & 16) //Qy est anime
	{
	//	std::cout << " Qy";
		animatedOrientation.y = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}
	if (flags & 32) //Qz est anime
	{
		animatedOrientation.z = pAnimations[iCurrentAnimation].pFrames[iFrame][sIndex+n];
		n++;
	}

	animatedOrientation.ComputeR();
	if (pJoint->iParent < 0) //pas de parent
	{
		pJoint->vPosition = animatedPosition;
		pJoint->qOrientation = animatedOrientation;
	}
	else //parent
	{
		pJoint->vPosition = q.Rotate(animatedPosition);
		pJoint->vPosition += v;
		pJoint->qOrientation = q*animatedOrientation;
	}

	for (int i = 0; i < pJoint->iNumChildrens; i++)
	{
		_BuildBone(iFrame,pJoint->pChildrens[i], pJoint->qOrientation, pJoint->vPosition);
	}
}

void MD5Mesh::SetAnimation(const std::string& name)
{
	int size = pAnimations.size();
	for (int i=0; i<size; i++) {
		if (pAnimations[i].sName == name)
			iCurrentAnimation = i;
			return;
	}
	//animation non-trouvée
	iCurrentAnimation = -1;
}

