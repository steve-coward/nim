#include "StdAfx.h"
#include "TGALoad.h"





_TGAHeader tga_header;				// Used To Store Our File Header
_TGA tga;					// Used To Store File Information

//CTGALoad::CTGALoad(void)
//{
//}


//CTGALoad::~CTGALoad(void)
//{
//}

// Load A TGA File!
bool LoadTGA(Texture *texture, char *filename)
{

	FILE * fTGA; // Declare File Pointer
	fTGA = fopen(filename, "rb"); // Open File For Reading

	// Uncompressed TGA Header
	GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
	// Compressed TGA Header
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

	if(fTGA == NULL)				// If Here Was An Error
	{
		//...Error code...
		return false;				// Return False
	}	
	// Attempt To Read The File Header
	if(fread(&tga_header, sizeof(_TGAHeader), 1, fTGA) == 0)
	{
		//...Error code here...
		return false;				// Return False If It Fails
	}	
	// If The File Header Matches The Uncompressed Header
	if(memcmp(uTGAcompare, &tga_header, sizeof(tga_header)) == 0)
	{
		// Load An Uncompressed TGA
		return LoadUncompressedTGA(texture, filename, fTGA);
	}
	// If The File Header Matches The Compressed Header
	else if(memcmp(cTGAcompare, &tga_header, sizeof(tga_header)) == 0)
	{											
		// Load A Compressed TGA
		return LoadCompressedTGA(texture, filename, fTGA);
	}
	else						// If It Doesn't Match Either One
	{
		//...Error code here...						
		return false;				// Return False
	}
}

// Load An Uncompressed TGA!
bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{
	// Attempt To Read Next 6 Bytes
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{										
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		//...Error code here...
		return false;				// Return False
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];	// Calculate Height
	texture->height = tga.header[3] * 256 + tga.header[2];	// Calculate The Width
	texture->bpp = tga.header[4];				// Calculate Bits Per Pixel
	tga.Width = texture->width;				// Copy Width Into Local Structure
	tga.Height = texture->height;				// Copy Height Into Local Structure
	tga.Bpp = texture->bpp;					// Copy Bpp Into Local Structure

	// Make Sure All Information Is Valid
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		//...Error code here...
		return false;				// Return False
	}

	if(texture->bpp == 24)				// Is It A 24bpp Image?
		texture->type	= GL_RGB;		// If So, Set Type To GL_RGB
	else						// If It's Not 24, It Must Be 32
		texture->type	= GL_RGBA;		// So Set The Type To GL_RGBA

	tga.bytesPerPixel = (tga.Bpp / 8);		// Calculate The BYTES Per Pixel
	// Calculate Memory Needed To Store Image
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);

	// Allocate Memory
	texture->imageData = (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL)			// Make Sure It Was Allocated Ok
	{
		//...Error code here...
		fclose(fTGA);														// Close the file
		return false;				// If Not, Return False
	}

	// Attempt To Read All The Image Data
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		if(texture->imageData != NULL)										// If imagedata has data in it
		{
			free(texture->imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		//...Error code here...								
		return false;				// If We Cant, Return False
	}

	// Start The Loop
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// 1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
			texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}

	fclose(fTGA);					// Close The File
	return true;					// Return Success
}

bool LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{ 
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		//...Error code here...
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];
	texture->height = tga.header[3] * 256 + tga.header[2];
	texture->bpp	= tga.header[4];
	tga.Width	= texture->width;
	tga.Height	= texture->height;
	tga.Bpp	= texture->bpp;

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		//...Error code here...
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
	}

	if(texture->bpp == 24)			// Is It A 24bpp Image?
		texture->type	= GL_RGB;	// If So, Set Type To GL_RGB
	else					// If It's Not 24, It Must Be 32
		texture->type	= GL_RGBA;	// So Set The Type To GL_RGBA

	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);

	// Allocate Memory To Store Image Data
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL) // If Memory Can Not Be Allocated...
	{
		//...Error code here...
		fclose(fTGA);														// Close file
		return false;				// Return False
	}

	GLuint pixelcount = tga.Height * tga.Width;	// Number Of Pixels In The Image
	GLuint currentpixel	= 0;			// Current Pixel We Are Reading From Data
	GLuint currentbyte	= 0;			// Current Byte We Are Writing Into Imagedata
	// Storage For 1 Pixel
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);

	// Start Loop
	do
	{
		GLubyte chunkheader = 0;			// Variable To Store The Value Of The Id Chunk
		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)	// Attempt To Read The Chunk's Header
		{
			//...Error code...
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(texture->imageData != NULL)									// If there is stored image data
			{
				free(texture->imageData);									// Delete image data
			}
			return false;				// If It Fails, Return False
		}

		if(chunkheader < 128)				// If The Chunk Is A 'RAW' Chunk
		{													
			chunkheader++;				// Add 1 To The Value To Get Total Number Of Raw Pixels
			
			// Start Pixel Reading Loop
			for(short counter = 0; counter < chunkheader; counter++)
			{
				// Try To Read 1 Pixel
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
				{
					//...Error code...
					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(texture->imageData != NULL)										// See if there is stored Image data
					{
						free(texture->imageData);										// If so, delete it too
					}
					return false;			// If It Fails, Return False
				}

				texture->imageData[currentbyte] = colorbuffer[2];		// Write The 'R' Byte
				texture->imageData[currentbyte + 1	] = colorbuffer[1];	// Write The 'G' Byte
				texture->imageData[currentbyte + 2	] = colorbuffer[0];	// Write The 'B' Byte
				if(tga.bytesPerPixel == 4)					// If It's A 32bpp Image...
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];	// Write The 'A' Byte
				}
				// Increment The Byte Counter By The Number Of Bytes In A Pixel
				currentbyte += tga.bytesPerPixel;
				currentpixel++;					// Increment The Number Of Pixels By 1
			}
		}
		else						// If It's An RLE Header
		{
			chunkheader -= 127;			// Subtract 127 To Get Rid Of The ID Bit

			// Read The Next Pixel
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
			{	
				//...Error code...
				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(texture->imageData != NULL)											// If thereis image data
				{
					free(texture->imageData);											// delete it
				}
				return false;				// If It Fails, Return False
			}

			// Start The Loop
			for(short counter = 0; counter < chunkheader; counter++)
			{
				// Copy The 'R' Byte
				texture->imageData[currentbyte] = colorbuffer[2];
				// Copy The 'G' Byte
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				// Copy The 'B' Byte
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)		// If It's A 32bpp Image
				{
					// Copy The 'A' Byte
					texture->imageData[currentbyte + 3] = colorbuffer[3];
				}
				currentbyte += tga.bytesPerPixel;	// Increment The Byte Counter
				currentpixel++;				// Increment The Pixel Counter
				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(texture->imageData != NULL)										// If there is Image data
					{
						free(texture->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
	} while(currentpixel < pixelcount);	// More Pixels To Read? ... Start Loop Over
			
	fclose(fTGA);				// Close File
	return true;				// Return Success
}











