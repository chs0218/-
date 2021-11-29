#ifndef _objRead
#define _objRead

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint s_program;
GLuint s_program_alpha;

GLuint v_shader_id;
GLuint f_shader_id;
GLuint f_shader_id_list[4];
GLuint index_buffer_id;		// Index Buffer Object


void checkCompileErrors(unsigned int shader, std::string type);
int MakeVertexShader(const char* vertexPath, int n);
int MakeFragmentShader(const char* fragmentPath, int count);


class ObjRead {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec2 > outuv;
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;
	float sizeX, sizeY, sizeZ;
public:
	std::vector< glm::vec3 > outvertex, outnormal;

	ObjRead() = default;
	~ObjRead() = default;

	int normalSize;
	int vertexSize;
	int loadObj(const char* filename);
	int loadObj_normalize_center(const char* filename);
};

void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

int MakeVertexShader(const char* vertexPath, int n)
{
	unsigned int ID;

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::ifstream vShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);

		std::stringstream vShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();

	// 2. compile shaders
	//unsigned int vertex, fragment;

	// vertex shader
	ID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ID, 1, &vShaderCode, NULL);
	glCompileShader(ID);
	checkCompileErrors(ID, "VERTEX");
	return ID;

}

int MakeFragmentShader(const char* fragmentPath, int count)
{
	unsigned int ID;

	// 1. retrieve the vertex/fragment source code from filePath

	std::string fragmentCode;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		fShaderFile.close();

		// convert stream into string
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	//unsigned int vertex, fragment;

	// fragment Shader
	ID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ID, 1, &fShaderCode, NULL);
	glCompileShader(ID);
	checkCompileErrors(ID, "FRAGMENT");

	return ID;
}
#endif
