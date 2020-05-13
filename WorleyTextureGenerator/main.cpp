/*
@author Hamza Mian
@date	May 13, 2019
@proj	This little program will generate a Texture3D file for OpenGL with a custom format.
		The format the same as Texture2D but with a 3rd dimension, much like having multiple 2D textures.
*/


#include <fstream>
#include <iostream>
#include "cWorleyTexture.h"

//#define VERIFY
#define SAFE_DELETE(ptr) if (ptr) { delete ptr; ptr = 0; }

int main()
{
	std::cout << "Generating texture..." << std::endl;

	/*

		IMPORTANT: THIS IS WHERE YOU CAN CHANGE THE RESOLUTION OF THE TEXTURE

	*/
	// TEXTURE IS OF RESOLUTION 256
	cWorleyTexture* worley = cWorleyTexture::Generate(256u, 4u, 15u, 30u);

	size_t width, height;
	unsigned char* data = worley->GetDataRGB(width, height);

	std::ofstream outfile("out.matrix", std::ios::binary);
	if (!outfile.is_open())
	{
		std::cout << "Woops! Couldn't open file :(" << std::endl;
		SAFE_DELETE(worley);
		return 0;
	}

	size_t cube = width * width * width * 3u;
	outfile << width;
	for (size_t i = 0; i < cube; ++i)
	{
		outfile << data[i];
	}

	outfile.close();

	std::cout << "Worley Texture file generated!" << std::endl;

#ifdef VERIFY
	std::cout << "Verifying texture integrity..." << std::endl;
	std::ifstream infile("out.matrix", std::ios::binary);
	if (!infile.is_open())
	{
		std::cout << "File is corrupt! :( can't open" << std::endl;
		SAFE_DELETE(worley);
		return 0;
	}

	size_t newWidth;
	infile >> newWidth;
	if (width != newWidth)
	{
		std::cout << "File is corrupt! :( wrong width" << std::endl;
		infile.close();
		SAFE_DELETE(worley);
		return 0;
	}
	else
	{
		std::cout << "Width = " << newWidth << std::endl;
	}

	char* newData = new char[newWidth * newWidth * newWidth * 3u];
	infile.read(newData, newWidth * newWidth * newWidth * 3u);

	for (size_t i = 0; i < newWidth * newWidth * newWidth * 3u; ++i)
	{
		unsigned char c = data[i];
		unsigned char temp = (unsigned char)newData[i];
		//infile >> temp;
		if (temp != c)
		{
			std::cout << "File is corrupt! :( wrong colours" << std::endl;
			infile.close();
			SAFE_DELETE(worley);
			return 0;
		}
	}

	infile.close();
	std::cout << "File successfully verified!" << std::endl;
#endif

	SAFE_DELETE(worley);
	return 0;
}