#ifndef Shader_h__
#define Shader_h__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
class CShader
{
public:
	CShader();
	CShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~CShader();
	void Use();
private:
	GLuint m_program;
};


#endif // Shader_h__


