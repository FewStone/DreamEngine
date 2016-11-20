#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <GL/gl3w.h>

namespace TextureNS
{
	struct header
	{
		unsigned char       identifier[12];
		unsigned int        endianness;
		unsigned int        gltype;
		unsigned int        gltypesize;
		unsigned int        glformat;
		unsigned int        glinternalformat;
		unsigned int        glbaseinternalformat;
		unsigned int        pixelwidth;
		unsigned int        pixelheight;
		unsigned int        pixeldepth;
		unsigned int        arrayelements;
		unsigned int        faces;
		unsigned int        miplevels;
		unsigned int        keypairbytes;
	};

	class Texture
	{
	public:
		Texture();
		~Texture();

		unsigned int LoadKTX(const char * filename, unsigned int tex = 0);

	private:
		

	};

}

