#ifndef _TGTEXTURE_H
#define _TGTEXTURE_H

#include "defs.h"

typedef unsigned char uint8;
typedef signed short sint16;
typedef signed char sint8;

enum texMode 
{
	REPLACE,
	BLEND,
	DECAL,
	MODULATE
};

enum texType
{
	TG_TEXTURE_1D,
	TG_TEXTURE_2D,
	TG_TEXTURE_3D
};

//DECOMPRESSION DES BMPS
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef struct {
	uint16 bfType;
	uint32 bfSize;
	uint16 bfReserved1;
	uint16 bfReserved2;
	uint32 bfOffBits;
} bmpFileHeader;

/** 
 * BITMAPINFOHEADER struct.
 */
typedef struct bmpHeader{
	uint32 biSize;
	uint32 biWidth;
	uint32 biHeight;
	uint16 biPlanes;
	uint16 biBitCount;
	uint32 biCompression;
	uint32 biSizeImage;
	uint32 biXPelsPerMeter;
	uint32 biYPelsPerMeter;
	uint32 biClrUsed;
	uint32 biClrImportant;
} ;

enum TGATypes {
	/// Uncompressed RGB image.
	TGA_RGB = 2,
	/// Uncompressed greyscale image.
	TGA_GREY = 3,
	/// Run-lenght encoded RGB image.
	TGA_RLERGB = 10,
	/// Run-lenght encoded greyscale image.
	TGA_RLEGREY = 11
};

struct tgaHeader{
	uint8 imageIDLength;
	uint8 colorMapType;
	uint8 imageTypeCode;
	sint8 colorMapOrigin;
	sint16 colorMapLength;
	sint16 colorMapEntrySize;
	sint16 imageXOrigin;
	sint16 imageYOrigin;
	sint16 imageWidth;
	sint16 imageHeight;
	uint8 bitCount;
	uint8 imageDescriptor;
} ;

class Texture 
{
 public:
 //friend class TextureManager;
 //protected:
	Texture ();
	~Texture();
 	bool LoadArray (unsigned char*, int, int, texType);
	bool LoadTexture (char*, texType);
	bool LoadBMP (char*, texType);
	bool LoadTGA (char*, texType);
#ifdef TGSDL_IMAGE
	bool LoadJPG (char*, texType);
	bool LoadPNG (char*, texType);
#endif
	bool LoadWhite ();
	void FlipAroundY (int, int, unsigned char*);
	void FlipAroundX (int, int, unsigned char*);
	texType GetTexType () { return tType; }
	unsigned int iId;
 protected:
	unsigned char* cData;
	unsigned int iWidth;
	unsigned int iHeight;
	unsigned int iBpp;
	texMode tMode;
	texType tType;
	
	bool bLoaded; //a-t-elle deja ete chargee (si oui, on doit faire glDeleteTexture avant de recharger autre chose
};

#endif
