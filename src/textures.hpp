#pragma once

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint setup_texture(const char* filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w, h, chan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);

	if (pxls)
	{

		if (chan == 4) {

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
		}
		else if (chan == 3) {

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);

		}
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] pxls;
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	return texObject;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < faces.size(); i++)
	{	
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	stbi_set_flip_vertically_on_load(true);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


GLuint setup_mipmaps(const char* filename[], int n)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int w[16], h[16], chan[16];
	unsigned char* pxls[16];
	stbi_set_flip_vertically_on_load(true);
	for (int c = 0; c < n; c++)
	{
		pxls[c] = stbi_load(filename[c], &w[c], &h[c], &chan[c], 0);
		if (pxls)
		{
			glTexImage2D(GL_TEXTURE_2D, c, GL_RGB, w[c], h[c], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[c]);
		}
		delete[] pxls[c];
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}
