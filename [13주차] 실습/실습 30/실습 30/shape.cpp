#include "shape.h"

void InitVertices()
{
	for (int i = 0; i < 6; ++i)
	{
		shapevertices[30 * i] = Dots[Shapeindex[6 * i]][0];
		shapevertices[30 * i + 1] = Dots[Shapeindex[6 * i]][1];
		shapevertices[30 * i + 2] = Dots[Shapeindex[6 * i]][2];
		shapevertices[30 * i + 3] = textureST[textureindex[0]][0];
		shapevertices[30 * i + 4] = textureST[textureindex[0]][1];
					  
		shapevertices[30 * i + 5] = Dots[Shapeindex[6 * i + 1]][0];
		shapevertices[30 * i + 6] = Dots[Shapeindex[6 * i + 1]][1];
		shapevertices[30 * i + 7] = Dots[Shapeindex[6 * i + 1]][2];
		shapevertices[30 * i + 8] = textureST[textureindex[1]][0];
		shapevertices[30 * i + 9] = textureST[textureindex[1]][1];
					  
		shapevertices[30 * i + 10] = Dots[Shapeindex[6 * i + 2]][0];
		shapevertices[30 * i + 11] = Dots[Shapeindex[6 * i + 2]][1];
		shapevertices[30 * i + 12] = Dots[Shapeindex[6 * i + 2]][2];
		shapevertices[30 * i + 13] = textureST[textureindex[2]][0];
		shapevertices[30 * i + 14] = textureST[textureindex[2]][1];
					  
		shapevertices[30 * i + 15] = Dots[Shapeindex[6 * i + 3]][0];
		shapevertices[30 * i + 16] = Dots[Shapeindex[6 * i + 3]][1];
		shapevertices[30 * i + 17] = Dots[Shapeindex[6 * i + 3]][2];
		shapevertices[30 * i + 18] = textureST[textureindex[3]][0];
		shapevertices[30 * i + 19] = textureST[textureindex[3]][1];
					  
		shapevertices[30 * i + 20] = Dots[Shapeindex[6 * i + 4]][0];
		shapevertices[30 * i + 21] = Dots[Shapeindex[6 * i + 4]][1];
		shapevertices[30 * i + 22] = Dots[Shapeindex[6 * i + 4]][2];
		shapevertices[30 * i + 23] = textureST[textureindex[4]][0];
		shapevertices[30 * i + 24] = textureST[textureindex[4]][1];
					  
		shapevertices[30 * i + 25] = Dots[Shapeindex[6 * i + 5]][0];
		shapevertices[30 * i + 26] = Dots[Shapeindex[6 * i + 5]][1];
		shapevertices[30 * i + 27] = Dots[Shapeindex[6 * i + 5]][2];
		shapevertices[30 * i + 28] = textureST[textureindex[5]][0];
		shapevertices[30 * i + 29] = textureST[textureindex[5]][1];
	}

	for (int i = 0; i < 4; ++i)
	{
		shapevertices[15 * i + 0 + 180] = Dots[Shapeindex[3 * i + 36]][0];
		shapevertices[15 * i + 1 + 180] = Dots[Shapeindex[3 * i + 36]][1];
		shapevertices[15 * i + 2 + 180] = Dots[Shapeindex[3 * i + 36]][2];
		shapevertices[15 * i + 3 + 180] = textureST[4][0];
		shapevertices[15 * i + 4 + 180] = textureST[4][1];

		shapevertices[15 * i + 5 + 180] = Dots[Shapeindex[3 * i + 1 + 36]][0];
		shapevertices[15 * i + 6 + 180] = Dots[Shapeindex[3 * i + 1 + 36]][1];
		shapevertices[15 * i + 7 + 180] = Dots[Shapeindex[3 * i + 1 + 36]][2];
		shapevertices[15 * i + 8 + 180] = textureST[0][0];
		shapevertices[15 * i + 9 + 180] = textureST[0][1];

		shapevertices[15 * i + 10 + 180] = Dots[Shapeindex[3 * i + 2 + 36]][0];
		shapevertices[15 * i + 11 + 180] = Dots[Shapeindex[3 * i + 2 + 36]][1];
		shapevertices[15 * i + 12 + 180] = Dots[Shapeindex[3 * i + 2 + 36]][2];
		shapevertices[15 * i + 13 + 180] = textureST[1][0];
		shapevertices[15 * i + 14 + 180] = textureST[1][1];
	}

	for (int i = 0; i < 48; ++i)
	{
		std::cout << i << "¹øÂ°: (" << shapevertices[5 * i] << ", " << shapevertices[5 * i + 1] << ", " << shapevertices[5 * i + 2] << "), (" << shapevertices[5 * i + 3] << ", " << shapevertices[5 * i + 4] << ")" << std::endl;
	}
}

void InitTexture()
{
	int widthImage, heightImage, numberOfChannel;

	glGenTextures(6, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("A.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("B.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("C.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("D.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("E.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("F.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("background.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}