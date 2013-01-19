#ifndef __TGA_H__
#define __TGA_H__

#include <windows.h> // Standard Windows header
#include <stdio.h> // Standard Header For File I/O
#include <gl\gl.h> // Standard Header For OpenGL
#include "Texture.h"

#pragma comment(lib, "OpenGL32.lib") // Link To Opengl32.lib

struct TGAHeader
{
	GLubyte Header[12];			// File Header To Determine File Type
};


struct TGA
{
	GLubyte header[6];			// Holds The First 6 Useful Bytes Of The File
	GLuint bytesPerPixel;			// Number Of BYTES Per Pixel (3 Or 4)
	GLuint imageSize;			// Amount Of Memory Needed To Hold The Image
	GLuint type;				// The Type Of Image, GL_RGB Or GL_RGBA
	GLuint Height;				// Height Of Image					
	GLuint Width;				// Width Of Image				
	GLuint Bpp;				// Number Of BITS Per Pixel (24 Or 32)
};

extern  TGAHeader tgaheader; // TGA header
extern TGA tga; // TGA image data


#endif
