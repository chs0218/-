#include "shape.h"

void InitVertices()
{
	for (int i = 0; i < 16; ++i)
	{
		shapevertices[18 * i] = Dots[Shapeindex[3 * i]][0];
		shapevertices[18 * i + 1] = Dots[Shapeindex[3 * i]][1];
		shapevertices[18 * i + 2] = Dots[Shapeindex[3 * i]][2];
		shapevertices[18 * i + 3] = normal[normalindex[3 * i]][0];
		shapevertices[18 * i + 4] = normal[normalindex[3 * i]][1];
		shapevertices[18 * i + 5] = normal[normalindex[3 * i]][2];

		shapevertices[18 * i + 6] = Dots[Shapeindex[3 * i + 1]][0];
		shapevertices[18 * i + 7] = Dots[Shapeindex[3 * i + 1]][1];
		shapevertices[18 * i + 8] = Dots[Shapeindex[3 * i + 1]][2];
		shapevertices[18 * i + 9] = normal[normalindex[3 * i + 1]][0];
		shapevertices[18 * i + 10] = normal[normalindex[3 * i + 1]][1];
		shapevertices[18 * i + 11] = normal[normalindex[3 * i + 1]][2];

		shapevertices[18 * i + 12] = Dots[Shapeindex[3 * i + 2]][0];
		shapevertices[18 * i + 13] = Dots[Shapeindex[3 * i + 2]][1];
		shapevertices[18 * i + 14] = Dots[Shapeindex[3 * i + 2]][2];
		shapevertices[18 * i + 15] = normal[normalindex[3 * i + 2]][0];
		shapevertices[18 * i + 16] = normal[normalindex[3 * i + 2]][1];
		shapevertices[18 * i + 17] = normal[normalindex[3 * i + 2]][2];
	}
	for (int i = 0; i < 48; ++i)
	{
		std::cout << i << "¹øÂ°: (" << shapevertices[6 * i] << ", " << shapevertices[6 * i + 1] << ", " << shapevertices[6 * i + 2] << "), (" << shapevertices[6 * i + 3] << ", " << shapevertices[6 * i + 4] << ", " << shapevertices[6 * i + 5] << ")" << std::endl;
	}
}

void InitDots()
{
	int iDegree = 0;
	for (int i = 0; i < 360; ++i)
	{
		OrbitDots[i][0] = 1.0 * (GLfloat)cos(iDegree * PI / 180.0f);
		OrbitDots[i][2] = 1.0 * (GLfloat)sin(iDegree * PI / 180.0f);
		iDegree = (iDegree + 1) % 360;
	}
}