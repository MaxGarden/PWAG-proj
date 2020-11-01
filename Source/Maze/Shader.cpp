#include "Shader.h"

Shader::Shader(const Camera& camera, const char* vertexShaderFile, const char* fragmentShaderFile) :
    m_camera {camera}
{
    if(vertexShaderFile != nullptr && fragmentShaderFile != nullptr)
        SetShaders(vertexShaderFile, fragmentShaderFile);
}

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgramHandle);
}

void Shader::SetColor(const glm::vec3 &color) const noexcept
{
    SetVec3("color", color);
}

void Shader::SetModelMatrix(const glm::mat4& modelMatrix) const noexcept
{
    SetMat4("modelMatrix", modelMatrix);
}

char* Shader::ReadShader(const char* aShaderFile)
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

int Shader::SetVertexShader(const char* vertexShaderFile)
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

int Shader::SetFragmentShader(const char* fragmentShaderFile)
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

void Shader::SetVec3(const char* name, const glm::vec3& value) const noexcept
{
    glUniform3f(glGetUniformLocation(m_shaderProgramHandle, name), value.x, value.y, value.z);
}

void Shader::SetMat4(const char* name, const glm::mat4& value) const noexcept
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramHandle, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetFloat(const char* name, float value) const noexcept
{
    glUniform1f(glGetUniformLocation(m_shaderProgramHandle, name), value);
}

void Shader::Use() const
{
	glUseProgram(m_shaderProgramHandle);
}

void Shader::SetShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	int vertexShadeHandle = SetVertexShader(vertexShaderFile);
	int fragmentShadeHandle = SetFragmentShader(fragmentShaderFile);

	if (vertexShadeHandle == 0 || fragmentShadeHandle == 0)
		return ;

	m_shaderProgramHandle = glCreateProgram();
	glAttachShader(m_shaderProgramHandle, vertexShadeHandle);
	glAttachShader(m_shaderProgramHandle, fragmentShadeHandle);
	glLinkProgram(m_shaderProgramHandle);

	int success;
	char infoLog[512];
	glGetProgramiv(m_shaderProgramHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgramHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShadeHandle);
	glDeleteShader(fragmentShadeHandle);
}

void Shader::Update() const
{
    const auto projectionMatrix = m_camera.GetProjectionMatrix();
    const auto viewMatrix = m_camera.GetViewMatrix();

    SetMat4("viewMatrix", viewMatrix);
    SetMat4("projectionMatrix", projectionMatrix);
    
    const auto cameraPosition = m_camera.GetPosition();
    const auto cameraFrontDirection = m_camera.GetFrontDirection();
    
    SetVec3("light.position", cameraPosition);
    SetVec3("light.frontDirection", cameraFrontDirection);
    SetVec3("light.ambient", glm::vec3{0.1f, 0.1f, 0.1f});
    SetVec3("light.diffuse", glm::vec3{0.8f, 0.8f, 0.8f});
            
    SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
}
