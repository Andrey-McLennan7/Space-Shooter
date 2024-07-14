#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	const char* vertexShaderData;
	const char* fragmentShaderData;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open Files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		// Read Files
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// Close Files
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Read as a string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

		// Succesfully read files
		this->m_success = true;
	}
	catch (std::ifstream::failure e)
	{
		this->m_error_message = "FAILED TO READ FILES";
		this->m_success = false;
		return;
	}

	// Read as constant character
	vertexShaderData = vertexCode.c_str();
	fragmentShaderData = fragmentCode.c_str();

	// Vertex
	this->CompileShader(this->m_vertex, vertexShaderData, GL_VERTEX_SHADER, "VERTEX");

	// Fragment
	this->CompileShader(this->m_fragment, fragmentShaderData, GL_FRAGMENT_SHADER, "FRAGMENT");

	// Program
	this->CreateShaderProgram();
}

Shader::~Shader()
{}

void Shader::CompileShader(GLuint& shader, const char* shaderData, GLenum type)
{
	this->CompileShader(shader, shaderData, type, "");
}

void Shader::CompileShader(GLuint& shader, const char* shaderData, GLenum type, std::string shaderName)
{
	int success{ 0 };
	char infolog[1024];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderData, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infolog);
		std::cerr << "FAILED::SHADER::" << shaderName << " " << infolog << std::endl;
		return;
	}
}

void Shader::CreateShaderProgram()
{
	int success{ 0 };
	char infolog[1024];

	this->m_id = glCreateProgram();

	glAttachShader(this->m_id, this->m_vertex);
	glAttachShader(this->m_id, this->m_fragment);
	glLinkProgram(this->m_id);

	glGetProgramiv(this->m_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(this->m_id, 1024, NULL, infolog);
		std::cerr << "FAILED::SHADER::PROGRAM " << infolog << std::endl;
		return;
	}

	glDeleteShader(this->m_vertex);
	glDeleteShader(this->m_fragment);
}

void Shader::use() const
{
	glUseProgram(this->m_id);
}

void Shader::unuse() const
{
	glUseProgram(0);
}

void Shader::setFloat(const std::string& attribute, float value) const
{
	this->use();
	glUniform1f(glGetUniformLocation(this->m_id, attribute.c_str()), (GLfloat)value);
	this->unuse();
}

void Shader::setInt(const std::string& attribute, int value) const
{
	this->use();
	glUniform1i(glGetUniformLocation(this->m_id, attribute.c_str()), (GLint)value);
	this->unuse();
}

void Shader::setBool(const std::string& attribute, bool value) const
{
	this->setInt(attribute, (int)value);
}

void Shader::setVec3(const std::string& attribute, float x, float y, float z) const
{
	this->use();
	glUniform3fv(glGetUniformLocation(this->m_id, attribute.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
	this->unuse();
}

void Shader::setVec3(const std::string& attribute, const glm::vec3& value) const
{
	this->setVec3(attribute, value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& attribute, const float* value) const
{
	this->setVec3(attribute, value[0], value[1], value[2]);
}

void Shader::setVec4(const std::string& attribute, float x, float y, float z, float w) const
{
	this->use();
	glUniform4fv(glGetUniformLocation(this->m_id, attribute.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w)));
	this->unuse();
}

void Shader::setVec4(const std::string& attribute, const glm::vec4& value) const
{
	this->setVec4(attribute, value.x, value.y, value.z, value.w);
}

void Shader::setVec4(const std::string& attribute, const float* value) const
{
	this->setVec4(attribute, value[0], value[1], value[2], value[3]);
}

void Shader::setMat4(const std::string& attribute, const glm::mat4& value) const
{
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->m_id, attribute.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	this->unuse();
}