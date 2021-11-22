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

void Sierpinski(GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat Normal[], int n)
{
	GLfloat V01[3], V12[3], V20[3];
	if (n > 0)
	{
		V01[0] = (V1[0] + V2[0]) / 2.0;
		V01[1] = (V1[1] + V2[1]) / 2.0;
		V01[2] = (V1[2] + V2[2]) / 2.0;

		V12[0] = (V2[0] + V3[0]) / 2.0;
		V12[1] = (V2[1] + V3[1]) / 2.0;
		V12[2] = (V2[2] + V3[2]) / 2.0;
							   
		V20[0] = (V3[0] + V1[0]) / 2.0;
		V20[1] = (V3[1] + V1[1]) / 2.0;
		V20[2] = (V3[2] + V1[2]) / 2.0;

		Sierpinski(V1, V01, V20, Normal, n - 1);
		Sierpinski(V2, V12, V01, Normal, n - 1);
		Sierpinski(V3, V20, V12, Normal, n - 1);
	}

	else
	{
		float Sierpinskivertices[] = {
			V1[0], V1[1], V1[2], Normal[0], Normal[1], Normal[2],
			V2[0], V2[1], V2[2], Normal[0], Normal[1], Normal[2],
			V3[0], V3[1], V3[2], Normal[0], Normal[1], Normal[2],
		};
		glBindVertexArray(SierpinskiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, SierpinskiVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Sierpinskivertices), Sierpinskivertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		DrawTriangle();
	}
}

void DrawTriangle()
{
	glBindVertexArray(SierpinskiVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}