#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <random>


void main()	//---윈도우 출력하고 콜백함수 설정
{
	glm::vec3 tmp1(1.0, 0.0, 0.5);
	glm::vec3 tmp2(0.5, -0.5, 0.0);
	glm::vec3 tmp3 = tmp1 - tmp2;
	std::cout << tmp3.x << std::endl;
}
