#ifndef Render_h__
#define Render_h__

#include "Shader.h"
#include <GL\glew.h>
class CShader;
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
	CShader* m_shader;

};

#endif // Render_h__


