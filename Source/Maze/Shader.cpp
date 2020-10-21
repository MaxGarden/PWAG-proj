#include "Shader.h"


char* Shader::ReadShader(char* aShaderFile)
{
	FILE* filePointer = fopen(aShaderFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

int Shader::SetVertexShader(char* vertexShaderFile)
{
	char* vertexShader = ReadShader(vertexShaderFile);
	int vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderHandle, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderHandle);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return vertexShaderHandle;
}

int Shader::SetFragmentShader(char* fragmentShaderFile)
{
	char* fragmentShader = ReadShader(fragmentShaderFile);
	int fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderHandle, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderHandle);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return fragmentShaderHandle;
}

Shader::Shader() {}

Shader::Shader(char* vertexShaderFile, char* fragmentShaderFile)
{
	SetShaders(vertexShaderFile, fragmentShaderFile);
}

Shader::~Shader()
{
	//glDeleteProgram(shaderProgramHandle);
}

void Shader::SetVec3(const char* name, float x, float y , float z)
{
	glUniform3f(glGetUniformLocation(shaderProgramHandle, name), x, y, z);
}

void Shader::setMat4(const char* name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramHandle, name), 1, GL_FALSE, &mat[0][0]);
}


void Shader::Use()
{
	glUseProgram(shaderProgramHandle);
}

void Shader::SetShaders(char* vertexShaderFile, char* fragmentShaderFile)
{
	int vertexShadeHandle = SetVertexShader(vertexShaderFile);
	int fragmentShadeHandle = SetFragmentShader(fragmentShaderFile);

	if (vertexShadeHandle == 0 || fragmentShadeHandle == 0)
		return ;

	shaderProgramHandle = glCreateProgram();
	glAttachShader(shaderProgramHandle, vertexShadeHandle);
	glAttachShader(shaderProgramHandle, fragmentShadeHandle);
	glLinkProgram(shaderProgramHandle);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShadeHandle);
	glDeleteShader(fragmentShadeHandle);
}

void Shader::SetMVP()
{
	glUseProgram(shaderProgramHandle);
	//zmienna typu UNIFORM -- macierz przekształcenia

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	glm::mat4 Tr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 Rt = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//macierz przekształcenia
	glm::mat4 mvp = Projection * Tr * Rt;
	setMat4("MVP", mvp);

}

