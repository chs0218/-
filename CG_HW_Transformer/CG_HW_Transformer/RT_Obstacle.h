#ifndef _RT_Obstacle
#define _RT_Obstacle

#define OBS_NUM 4

class RT_Obstacle {
	GLfloat x = 0.0f, z = 0.0f;
	glm::mat4 transformMatrix = glm::mat4(1.0f);
public:
	RT_Obstacle() {
		x = 0.0f;
		z = 0.0f;
		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x, 0.0, z));
		transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(45.0), glm::vec3(-1.0, 0.0, 1.0));
		transformMatrix = glm::translate(transformMatrix, glm::vec3(0.5 * BOXSIZE, 0.5 * BOXSIZE, 0.5 * BOXSIZE));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5, 0.5, 0.5));
	}

	RT_Obstacle(GLfloat transX, GLfloat transZ)
	{
		x = transX;
		z = transZ;
		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x, 0.0, z));
		transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(45.0), glm::vec3(-1.0, 0.0, 1.0));
		transformMatrix = glm::translate(transformMatrix, glm::vec3(0.5 * BOXSIZE, 0.5 * BOXSIZE, 0.5 * BOXSIZE));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5, 0.5, 0.5));
	}

	void update()
	{
		transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(5.0), glm::vec3(1.0, 1.0, 1.0));
	}

	glm::mat4 ReturnM() {
		return transformMatrix;
	}
};

RT_Obstacle rt_Obs[OBS_NUM] = { RT_Obstacle(-7.0, 7.0), RT_Obstacle(-7.0, -7.0), RT_Obstacle(7.0, 7.0), RT_Obstacle(7.0, -7.0) };

#endif
