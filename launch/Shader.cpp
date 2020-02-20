#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

CShader::CShader()
{
	m_program = glCreateProgram();
}

CShader::~CShader()
{
	glDeleteProgram(m_program);
}

void CShader::Use()
{
	glUseProgram(m_program);
}

void CShader::SetVertexShader(const std::string &file_path)
{
	m_vertex_shader = CreateShader(GL_VERTEX_SHADER, file_path);
}

void CShader::SetFragmentShader(const std::string &file_path)
{
	m_fragment_shader = CreateShader(GL_FRAGMENT_SHADER, file_path);
}

void CShader::LinkShader()
{
	glAttachShader(m_program, m_vertex_shader);
	glAttachShader(m_program, m_fragment_shader);
	glLinkProgram(m_program);
	GLint success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	char infoLog[1024];

	if (!success)
	{
		glGetProgramInfoLog(m_program, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//assert(success);
	//ÊÍ·Å
	glDeleteShader(m_vertex_shader);
	glDeleteShader(m_fragment_shader);
}

GLuint CShader::CreateShader(GLuint shader_type, const std::string &file_path)
{
	GLuint returnShader;
	std::string vertexCode;
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	try
	{
		file.open(file_path);
		std::stringstream vShaderStream;
		vShaderStream << file.rdbuf();
		file.close();
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* temp = vertexCode.c_str();
	GLint success;
	returnShader = glCreateShader(shader_type);
	glShaderSource(returnShader, 1, &temp, NULL);
	glCompileShader(returnShader);
	glGetShaderiv(returnShader, GL_COMPILE_STATUS, &success);
	assert(success);
	return returnShader;
}
