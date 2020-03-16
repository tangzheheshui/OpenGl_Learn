#pragma once

// Std. Includes
#include <iostream>
#include <map>
#include <string>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
// GL includes
#include "Shader.h"


struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class CShader;

class GlFont
{
public:
	GlFont();
	~GlFont();
public:
	void RenderText(CShader* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	void CreateCharacters();
private:
	std::map<GLchar, Character> Characters;
};

