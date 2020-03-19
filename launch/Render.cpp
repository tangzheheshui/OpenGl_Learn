#include "Render.h"
#include <string>
#include <iostream>
#include <SOIL/SOIL.h>
#include <assert.h>
#include <GLFW/glfw3.h>
#include "Font.h"
#include "resource_manager.h"
#include "SpriteRenderer.h"


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

CRender::CRender()
{
	//3D场景的着色器
	ResourceManager::LoadShader("../shader/shader.vs", "../shader/shader.frag", "", "shader_3d");
	Init3DShaderUniform(ResourceManager::GetShader("shader_3d"));
	Create3DVaoVbo();

	//文字绘制着色器
	ResourceManager::LoadShader("../shader/shader_font.vs", "../shader/shader_font.frag", "", "shader_font");
	ResourceManager::GetShader("shader_font")->Use();
	InitFontShaderUniform(ResourceManager::GetShader("shader_font"));
	CreateFontVaoVbo();

	//字体绘制
	m_font = new GlFont;

	//加载纹理
	ResourceManager::LoadTexture("../img/awesomeface.png", GL_TRUE, "face");

	//精灵渲染器
	m_sprite_renderer = new SpriteRenderer;

	//光照的着色器
	ResourceManager::LoadShader("../shader/light.vs", "../shader/light.frag", "", "shader_light");
	ResourceManager::LoadShader("../shader/light_cube.vs", "../shader/light_cube.frag", "", "shader_light_cube");
	CreateLightVaoVbo();
}

CRender::~CRender()
{

}

CRender* CRender::Instance()
{
	static CRender instance;
	return &instance;
}

void CRender::Update()
{
	const float PI = 3.1415926 / 180;
	float radius = 10.0f;
	double time = glfwGetTime();
//	time = 0;
	float camX = sin(time) * radius;
	float camZ = cos(time) * radius;
	float yaw = (time + 3.1415926) / PI;
	m_camera.SetCamera(glm::vec3(camX, 0.0f, camZ), 0.0, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	ResourceManager::GetShader("shader_3d")->Use();
	ResourceManager::GetShader("shader_3d")->setMat4("view", m_camera.GetViewMatrix());
}

void CRender::Render()
{
	//清理颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制3D
	ResourceManager::GetShader("shader_3d")->Use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureA);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureB);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ResourceManager::GetShader("shader_3d")->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//文字
	glBindVertexArray(VAO_font);
	ResourceManager::GetShader("shader_font")->SetVAO(VAO_font);
	ResourceManager::GetShader("shader_font")->SetVBO(VBO_font);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_font->RenderText(ResourceManager::GetShader("shader_font"), "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

	//绘制精灵
	if (m_sprite_renderer)
	{
		m_sprite_renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void CRender::RenderLightTest()
{
	//清理颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CShader* lightingShader = ResourceManager::GetShader("shader_light_cube");
	lightingShader->Use();
	lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	// view/projection transformations
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = m_camera.GetViewMatrix();
	lightingShader->setMat4("projection", projection);
	lightingShader->setMat4("view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	lightingShader->setMat4("model", model);

	// render the cube
	glBindVertexArray(VAO_light_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// also draw the lamp object
	CShader* lampShader = ResourceManager::GetShader("shader_light");
	
	lampShader->Use();
	lampShader->setMat4("projection", projection);
	lampShader->setMat4("view", view);
	model = glm::mat4(1.0f);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	lampShader->setMat4("model", model);

	glBindVertexArray(VAO_light);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CRender::DeleteBuff()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

GLuint CRender::CreateTexture(const std::string &path)
{
	GLuint texture = 0;
	if (path.empty())
	{
		return texture;
	}
	glGenTextures(1, &texture);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	//纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	assert(image);
	//生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放内存
	SOIL_free_image_data(image);
	return texture;
}

void CRender::CreateTexture()
{
	m_textureA = CreateTexture("../img/container.jpg");
	m_textureB = CreateTexture("../img/awesomeface.png");
}

void CRender::Create3DVaoVbo()
{
	//创建VAO VBO 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//绑定VAO
	glBindVertexArray(VAO);

	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void CRender::CreateLightVaoVbo()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	// first, configure the cube's VAO (and VBO)
	
	glGenVertexArrays(1, &VAO_light_cube);
	glGenBuffers(1, &VBO_light);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO_light_cube);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO_light);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void CRender::Init3DShaderUniform(CShader* shader)
{
	if (!shader)
	{
		return;
	}
	shader->Use();
	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
}

void CRender::InitFontShaderUniform(CShader* shader)
{
	if (!shader)
	{
		return;
	}
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void CRender::CreateFontVaoVbo()
{
	glGenVertexArrays(1, &VAO_font);
	glGenBuffers(1, &VBO_font);
	glBindVertexArray(VAO_font);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
