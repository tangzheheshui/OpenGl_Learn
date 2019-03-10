#pragma once
#include <GL\glew.h>
class CRender
{
public:
	CRender();
	~CRender();
public:
	static CRender* Instance();
	void Render();
	void DeleteBuff();
private:
	static CRender* m_instance;
	GLuint VBO, VAO, EBO;
	GLuint m_shaderProgram;
};

