#include "GLProgram.h"
#include <GL/glew.h>
#include <fstream>
#include <vector>

GLProgram::GLProgram()
{
}

bool GLProgram::loadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	vertexShaderName = vertex_file_path;
	fragmentShaderName = fragment_file_path;

	std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
	if(vertexShaderStream.is_open())
	{
		std::string line = "";
		vertexShader = "";
		while(getline(vertexShaderStream, line))
			vertexShader += "\n" + line;
		vertexShaderStream.close();
	} else {
		return false;
	}

	std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
	if(fragmentShaderStream.is_open()){
		std::string line = "";
		fragmentShader = "";
		while(getline(fragmentShaderStream, line))
			fragmentShader += "\n" + line;
		fragmentShaderStream.close();
	} else {
		return false;
	}

	return true;
}


bool GLProgram::loadToGpu() {
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexShaderName.c_str());
	const char * vertexSourcePointer = vertexShader.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> vertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentShaderName.c_str());
	const char * fragmentSourcePointer = fragmentShader.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> fragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	glId = glCreateProgram();
	glAttachShader(glId, vertexShaderID);
	glAttachShader(glId, fragmentShaderID);
	glLinkProgram(glId);

	// Check the program
	glGetProgramiv(glId, GL_LINK_STATUS, &result);
	glGetProgramiv(glId, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
	glGetProgramInfoLog(glId, infoLogLength, NULL, &programErrorMessage[0]);
	fprintf(stdout, "%s\n", &programErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return glId != 0;
}

void GLProgram::unloadFromGpu() {
	if(glId)
		glDeleteTextures(1, &glId);

	glId = 0;
}

void GLProgram::select() {
	glUseProgram(glId);
}

void GLProgram::unselect() {
	glUseProgram(0);
}
