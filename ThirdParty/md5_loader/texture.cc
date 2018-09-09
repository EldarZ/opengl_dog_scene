#include "texture.h"

Texture::Texture()
{
	bLoaded = false;
}

Texture::~Texture()
{
	if (bLoaded)
		glDeleteTextures (1, &iId);
}

bool Texture::LoadTexture (char* image, texType type)
{
	if(bLoaded)
		glDeleteTextures (1, &iId);
	
	if (strstr(image, ".bmp") != NULL)
		return LoadBMP (image, type);
	if (strstr(image, ".tga") != NULL)
		return LoadTGA (image, type);
#ifdef TGSDL_IMAGE
	if (strstr(image, ".png") != NULL)
		return LoadPNG (image, type);
	if (strstr(image, ".jpeg") != NULL || strstr(image, ".jpg") != NULL)
		return LoadJPG (image, type);
	#endif
	return false;
}

bool Texture::LoadTGA (char* image, texType type)
{
	FILE *fd;
	tgaHeader tgaheader;
	unsigned char *tgadata = NULL;
	unsigned int index, colorMode;
	unsigned char colorSwap;
	unsigned long int tgasize;
	unsigned char *tempbuf;

	std::cout << "[TGE] Chargement du TGA : " << image << std::endl;
	fd = fopen(image, "rb");
	if(fd == NULL) 
	{
		return false; 
	}

	// Fill in the tgaheader struct. We do it the hard way like above..
	tempbuf = new unsigned char[18];
	fread(tempbuf, 18, 1, fd);
	memcpy(&tgaheader.imageIDLength, tempbuf, sizeof(tgaheader.imageIDLength));
	memcpy(&tgaheader.colorMapType, tempbuf + 1, sizeof(tgaheader.colorMapType));
	memcpy(&tgaheader.imageTypeCode, tempbuf + 2, sizeof(tgaheader.imageTypeCode));
	memcpy(&tgaheader.colorMapOrigin, tempbuf + 3, sizeof(tgaheader.colorMapOrigin));
	memcpy(&tgaheader.colorMapLength, tempbuf + 5, sizeof(tgaheader.colorMapLength));
	memcpy(&tgaheader.colorMapEntrySize, tempbuf + 7, sizeof(tgaheader.colorMapEntrySize));
	memcpy(&tgaheader.imageXOrigin, tempbuf + 9, sizeof(tgaheader.imageXOrigin));
	memcpy(&tgaheader.imageYOrigin, tempbuf + 11, sizeof(tgaheader.imageYOrigin));
	memcpy(&tgaheader.imageWidth, tempbuf + 12, sizeof(tgaheader.imageWidth));
	memcpy(&tgaheader.imageHeight, tempbuf + 14, sizeof(tgaheader.imageHeight));
	memcpy(&tgaheader.bitCount, tempbuf + 16, sizeof(tgaheader.bitCount));
	memcpy(&tgaheader.imageDescriptor, tempbuf + 17, sizeof(tgaheader.imageDescriptor));
	
	
	
	delete tempbuf;

	if((tgaheader.imageTypeCode != TGA_RGB) && (tgaheader.imageTypeCode != TGA_GREY) && (tgaheader.imageTypeCode != TGA_RLERGB) && (tgaheader.imageTypeCode != TGA_RLEGREY)) {
		std::cerr << "[TGE] LoadTGA: " << image << " doesn't look like a TGA image\n" << std::endl;
		fclose(fd);
		return false;
	}

	// Uncompressed 16-bit & RLE compressed image loading code taken from NeHe. Thanks!

	// Check if this image is regular uncompressed RGB or greyscale image
	if(tgaheader.imageTypeCode == TGA_RGB || tgaheader.imageTypeCode == TGA_GREY) {
		// Check if the image is 24 or 32-bit image
		if(tgaheader.bitCount == 24 || tgaheader.bitCount == 32) {
			colorMode = tgaheader.bitCount / 8;
			tgasize = tgaheader.imageWidth * tgaheader.imageHeight * colorMode;
			tgadata = new unsigned char[sizeof(unsigned char) * tgasize];

			// Read in the TGA image data
			fread(tgadata, sizeof(unsigned char), tgasize, fd);

			// Change BGR to RGB
			for(index = 0; index < tgasize; index += colorMode) {
				colorSwap = tgadata[index];
				tgadata[index] = tgadata[index + 2];
				tgadata[index + 2] = colorSwap;
			}
		} else if(tgaheader.bitCount == 16) {
			unsigned short pixels = 0;
			unsigned char r = 0, g = 0, b = 0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			colorMode = 3;
			tgasize = tgaheader.imageWidth * tgaheader.imageHeight * colorMode;
			tgadata = new unsigned char[sizeof(unsigned char) * tgasize];

			// Load in all the pixel data pixel by pixel
			for(int i = 0; i < tgaheader.imageWidth * tgaheader.imageHeight; i++) {
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, fd);

				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;

				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				tgadata[i * 3 + 0] = r;
				tgadata[i * 3 + 1] = g;
				tgadata[i * 3 + 2] = b;
			}
		} else {
			std::cerr << "[TGE] LoadTGA:" << image << " has bad or unsupported pixel format (" << tgaheader.bitCount << ")" << std::endl;
			fclose(fd);
			return false;
		}
	} else {
		// It must be a RLE compressed image

		// Create some variables to hold the rleID, current colors read, channels, & stride.
		unsigned char rleID = 0;
		int i = 0;
		int colorsRead = 0;
		colorMode = tgaheader.bitCount / 8;
		tgasize = tgaheader.imageWidth * tgaheader.imageHeight * colorMode;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		tgadata = new unsigned char[sizeof(unsigned char) * tgasize];
		unsigned char *pColors = new unsigned char[colorMode];

		// Load in all the pixel data
		while(i < tgaheader.imageWidth * tgaheader.imageHeight) {
			// Read in the current color count + 1
			fread(&rleID, sizeof(unsigned char), 1, fd);

			// Check if we don't have an encoded string of colors
			if(rleID < 128) {
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while(rleID) {
					// Read in the current color
					fread(pColors, sizeof(unsigned char) * colorMode, 1, fd);

					// Store the current pixel in our image array
					tgadata[colorsRead + 0] = pColors[2];
					tgadata[colorsRead + 1] = pColors[1];
					tgadata[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(tgaheader.bitCount == 32)
						tgadata[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += colorMode;
				}
			}
			// Else, let's read in a string of the same character
			else {
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				fread(pColors, sizeof(unsigned char) * colorMode, 1, fd);

				// Go and read as many pixels as are the same
				while(rleID) {
                   // Assign the current pixel to the current index in our pixel array
                   tgadata[colorsRead + 0] = pColors[2];
                   tgadata[colorsRead + 1] = pColors[1];
                   tgadata[colorsRead + 2] = pColors[0];

                   // If we have a 4 channel 32-bit image, assign one more for the alpha
                   if(tgaheader.bitCount == 32)
                       tgadata[colorsRead + 3] = pColors[3];

                   // Increase the current pixels read, decrease the amount
                   // of pixels left, and increase the starting index for the next pixel.
                   i++;
                   rleID--;
                   colorsRead += colorMode;
				}
			}
		}
		delete pColors;
	}

	fclose(fd);

	iWidth = tgaheader.imageWidth;
	iHeight = tgaheader.imageHeight;
	iBpp = tgaheader.bitCount;
	tType = type;
	cData = tgadata;
	
	glGenTextures (1, &iId);
	glBindTexture(GL_TEXTURE_2D, iId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//TODO: CLAMP TO EDGE FAIT BUGGUER LES MAPS
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	//priorite de la texture... ca ne semble pas vraiment utile
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1);
	//FIXME: On admet qu'on a du TGA en mode RGBA !!! C'est pas forcément toujours vrai...
	if(iBpp == 32)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, cData);
	if (iBpp == 24)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, cData);
	/*if (type == TG_TEXTURE_1D)
		glTexImage1D(GL_TEXTURE_1D, 0, iBpp, iWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, cData);
	else if (type == TG_TEXTURE_2D)
		glTexImage2D(GL_TEXTURE_2D, 0, iBpp, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, cData);*/
	
	std::cout << "[TGE] " << image <<" : chargement reussi" << std::endl;
	bLoaded = true;
	return true;
}

bool Texture::LoadBMP (char* image, texType type)
{
	FILE *fd;
	bmpFileHeader bfheader;
	bmpHeader bmpheader;
	unsigned char *bmpdata = NULL;
	int index = 0;
	unsigned char colorSwap;
	int bmpsize, colorMode;
	unsigned char *tempbuf;

	fd = fopen(image, "rb");
	if(fd == NULL) 
	{
		std::cerr << "[TGE] Impossible de charger le fichier " << image << std::endl;;
		return false;
	}

	// Fill in the bmpFileHeader struct. We do it the hard way instead of
	// fread(&bfheader, sizeof(bmpFileHeader), 1, fd) because of struct member aligment
	tempbuf = new unsigned char[14];
	fread(tempbuf, 14, 1, fd);
	memcpy(&bfheader.bfType, tempbuf, sizeof(bfheader.bfType));
	memcpy(&bfheader.bfSize, tempbuf + 2, sizeof(bfheader.bfSize));
	memcpy(&bfheader.bfReserved1, tempbuf + 6, sizeof(bfheader.bfReserved1));
	memcpy(&bfheader.bfReserved2, tempbuf + 8, sizeof(bfheader.bfReserved2));
	memcpy(&bfheader.bfOffBits, tempbuf + 10, sizeof(bfheader.bfOffBits));
	
	delete[] tempbuf;

	// Verify that this is a valid BMP file
	if(bfheader.bfType != 0x4D42) {
		fclose(fd);
		std::cerr << "[TGE] Le fichier " << image << " ne semble pas etre une image bmp valide : " << bfheader.bfType << std::endl;
		return false;
	}

	// Read the bmp info header
	fread(&bmpheader, sizeof(bmpHeader), 1, fd);

	// Check that this is a 24-bit image
	if(bmpheader.biBitCount == 24) {
		// Calculate bytes per pixel and the size of image data
		colorMode = bmpheader.biBitCount / 8;
		bmpsize = bmpheader.biWidth * bmpheader.biHeight * colorMode;

		// Malloc memory for image data
		bmpdata = new unsigned char[sizeof(unsigned char) * bmpsize];

		// Seek to start of bmp data
		fseek(fd, bfheader.bfOffBits, SEEK_SET);

		// Read the bmp image data
		fread(bmpdata, sizeof(unsigned char), bmpsize, fd);

		// Swap the RGB values since BMPs are stored in BGR
		for(index = 0; index < bmpsize; index += 3){
			colorSwap = bmpdata[index];
			bmpdata[index] = bmpdata[index + 2];
			bmpdata[index + 2] = colorSwap;
		}
	} else {
		fclose(fd);
		std::cerr << "[TGE] L'image " << image << " a un mauvais format de pixel" << std::endl;
		return false;
	}

	fclose(fd);
	iWidth = bmpheader.biWidth;
	iHeight = bmpheader.biHeight;
	iBpp = bmpheader.biBitCount;
	tType = type;
	cData = bmpdata;
	
	glGenTextures (1, &iId);
	glBindTexture(GL_TEXTURE_2D, iId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//TODO: CLAMP TO EDGE FAIT BUGGUER LES MAPS
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//priorite de la texture... ca ne semble pas vraiment utile
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, cData);
	//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, cData);
	
	bLoaded = true;
	return true;
}

bool Texture::LoadWhite ()
{
	float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glGenTextures(1, &iId);
	glBindTexture(GL_TEXTURE_2D, iId);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, 1, 1, GL_RGB, GL_FLOAT, white);
	return true;
}
#ifdef TGSDL_IMAGE
//Code emprunte a Ogre3D
void Texture::FlipAroundY (int w, int h, unsigned char* pixels)
{
	//on considere qu'on est en RGB8
	unsigned char* buffer = new unsigned char [w*h*3];
	unsigned char* dst = NULL;
	unsigned char* src = pixels;
	for (int y=0; y<h; y++)
	{
		int offset = ((y * w) + (w - 1)) * 3;
		dst = buffer;
		dst += offset;
		for (int x=0; x<w; x++)
		{
			memcpy (dst, src, sizeof(unsigned char)*3);
			dst -= 3;
			src += 3;
		}
	}
	memcpy (pixels, buffer, w*h*sizeof(unsigned char)*3);
	delete buffer;
}

void Texture::FlipAroundX (int w, int h, unsigned char* pixels)
{
	//on considère qu'on est en RGB8
	int rowSpan = w*3;
	unsigned char* buffer = new unsigned char [rowSpan*h];
	unsigned char* ptr1 = pixels, *ptr2 = buffer + ((h-1)*rowSpan);
	for (int i=0; i<h; i++)
	{
		memcpy (ptr2, ptr1, rowSpan);
		ptr1 += rowSpan;
		ptr2 -= rowSpan;
	}
	memcpy (pixels, buffer, rowSpan*h);
	delete buffer;
}


bool Texture::LoadPNG (char* image, texType type)
{
	std::cout << "[TGE] chargement de "<<image<<std::endl;
	SDL_Surface* texture;
	texture = IMG_Load(image);
	
	if (texture == NULL)
		return false;
	FlipAroundX (texture->w, texture->h, (unsigned char*)texture->pixels);	
//	FlipAroundX (texture->w, texture->h, (unsigned char*)texture->pixels);
//	texture->pixels = ChangeTextureOrientation(texture->w, texture->h, (unsigned char*)texture->pixels);
	
	glGenTextures (1, &iId);
	glBindTexture(GL_TEXTURE_2D, iId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//TODO: CLAMP TO EDGE FAIT BUGGUER LES MAPS
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//priorite de la texture... ca ne semble pas vraiment utile
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture->w, texture->h, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);
	
	return true;
}

bool Texture::LoadJPG (char* image, texType type)
{
	std::cout << "chargement de "<<image<<std::endl;
	
	SDL_Surface* texture;
	texture = IMG_Load(image);
	
	if (texture == NULL)
		return false;
	
	FlipAroundX (texture->w, texture->h, (unsigned char*)texture->pixels);
	
	glGenTextures (1, &iId);
	glBindTexture(GL_TEXTURE_2D, iId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//TODO: CLAMP TO EDGE FAIT BUGGUER LES MAPS
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//priorite de la texture... ca ne semble pas vraiment utile
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture->w, texture->h, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);
	
	return true;
}

#endif

bool Texture::LoadArray (unsigned char* data, int w, int h, texType t)
{
	glGenTextures (1, &iId);
	glBindTexture (GL_TEXTURE_2D, iId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//TODO: CLAMP TO EDGE FAIT BUGGUER LES MAPS
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	bLoaded = true;
	return true;
}

