#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"

class Material
{
 public:
	Material () { pTex1 = pTex2 = pNormal = NULL; }
	void SetFirstTexture (Texture* p) { pTex1 = p; }
	void SetSecondTexture (Texture* p) { pTex2 = p; }
	void SetNormalTexture (Texture* p) { pNormal = p; }
	
	bool HasPrimary () { return (bool)pTex1; }
	bool HasSecondary () { return (bool)pTex2; }
	bool HasNormal () { return (bool)pNormal; }
	
	Texture* GetFirstTexture () { return pTex1; }
	Texture* GetSecondTexture () { return pTex2; }
	Texture* GetNormalTexture () { return pNormal; }
 protected:
	Texture* pTex1;
	Texture* pTex2;
	Texture* pNormal;
};

#endif
