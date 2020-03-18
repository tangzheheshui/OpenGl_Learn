#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <SOIL/SOIL.h>
#include "Shader.h"

// Instantiate static variables
std::map<std::string, Texture2D*>    ResourceManager::Textures;

std::map<std::string, CShader*>   ResourceManager::Shaders;

CShader* ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

CShader* ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D* ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second->GetProgram());
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second->ID);
}

CShader* ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	CShader* shader = new CShader;
	shader->SetVertexShader(vShaderFile);
	shader->SetFragmentShader(fShaderFile);
	shader->LinkShader();
	return shader;
}

Texture2D* ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D* texture = new Texture2D;
	if (alpha)
	{
		texture->Internal_Format = GL_RGBA;
		texture->Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture->Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture->Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}