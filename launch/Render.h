#ifndef Render_h__
#define Render_h__

#include "Shader.h"
#include <GL\glew.h>
#include "Camera.h"

class CShader;
class GlFont;
class SpriteRenderer;

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
	void Create3DVaoVbo();
	void Init3DShaderUniform(CShader* shader);
	void InitFontShaderUniform(CShader* shader);
	void CreateFontVaoVbo();
private:
	GLuint VBO, VAO, EBO;
	GLuint m_shaderProgram;
	GLuint VBO_font, VAO_font;
	GLuint m_textureA;
	GLuint m_textureB;
	Camera m_camera;
	GlFont* m_font;
	SpriteRenderer* m_sprite_renderer;
};

#endif // Render_h__


