#ifndef Render_h__
#define Render_h__

#include "Shader.h"
#include <GL\glew.h>
#include "Camera.h"

class CShader;
class GlFont;

class CRender
{
public:
	CRender();
	~CRender();
public:
	static CRender* Instance();
	void CreateTexture();
	void Update();
	void Render();
	void DeleteBuff();
private:
	GLuint CreateTexture(const std::string &path);
	void CreateVaoVbo();
	void InitShaser(CShader* shader);
	void CreateFontProjection(CShader* shader);
	void CreateFontVaoVbo();
private:
	GLuint VBO, VAO, EBO;
	GLuint m_shaderProgram;
	CShader* m_shader;
	GLuint VBO_font, VAO_font;
	CShader* m_shader_font;
	GLuint m_textureA;
	GLuint m_textureB;
	Camera m_camera;
	GlFont* m_font;
};

#endif // Render_h__


