#ifndef __TGALoad_H__
#define __TGALoad_H__

//#include "tga.h"
#include "texture.h"
#include <windows.h> // Standard Windows header
#include <stdio.h> // Standard Header For File I/O
#include <gl\gl.h> // Standard Header For OpenGL

struct _TGAHeader
{
	GLubyte Header[12];			// File Header To Determine File Type
};

struct _TGA
{
	GLubyte header[6];			// Holds The First 6 Useful Bytes Of The File
	GLuint bytesPerPixel;			// Number Of BYTES Per Pixel (3 Or 4)
	GLuint imageSize;			// Amount Of Memory Needed To Hold The Image
	GLuint type;				// The Type Of Image, GL_RGB Or GL_RGBA
	GLuint Height;				// Height Of Image					
	GLuint Width;				// Width Of Image				
	GLuint Bpp;				// Number Of BITS Per Pixel (24 Or 32)
};

//class CTGALoad
//{
	extern _TGAHeader tga_header;				// Used To Store Our File Header
	extern _TGA tga;					// Used To Store File Information
	extern GLubyte uTGAcompare[12];
	extern GLubyte cTGAcompare[12];

//public:
//	CTGALoad(void);
//	~CTGALoad(void);

	bool LoadTGA(Texture *texture, char *filename);
	// Load An Uncompressed File
	bool LoadUncompressedTGA(Texture *texture, char *filename, FILE *);
	// Load A Compressed File
	bool LoadCompressedTGA(Texture *texture, char *filename, FILE *);
//};

#endif

