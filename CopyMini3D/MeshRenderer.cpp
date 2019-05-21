#include "MeshRenderer.h"
#include "GLHeader.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace Engine::Components;
MeshRenderer::MeshRenderer()
{
	Vertex a, b, c, d;
	a.pos = glm::vec3(-0.5, 0, 0);
	b.pos = glm::vec3(0.5, 0, 0);
	c.pos = glm::vec3(0, 1, 0);
	d.pos = glm::vec3(0, 0, 0.5);
	std::vector<Vertex> vertices = {
		a,b,c,d
	};
	std::vector<unsigned int> indices = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		1,3,2,
		0,3,1
	};
	Bind(vertices, indices);
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	static float eyex = 0.0f;
	static float eyey = 0.0f;
	static float eyez = -4.0f;
	glm::mat4 view = glm::lookAt(glm::vec3(3 * cos(eyex), eyey, 3 * sin(eyex)),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	setMat4("projection", projection);
	setMat4("view", view);
	setMat4("model", model);
	RenderMesh();
	return;
}

void MeshRenderer::Bind(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{

	ComplieShaders();
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	length = indices.size();
	//glGenVertexArrays(1, &va_pos_only);
	glGenVertexArrays(1, &va_pos_normal_tex);
	//glGenVertexArrays(1, &va_pos_normal_tex_tangent);

	//glGenBuffers(1, &vb_pos_only);
	glGenBuffers(1, &vb_pos_normal_tex);
	//glGenBuffers(1, &vb_pos_normal_tex_tangent);

	//glGenBuffers(1, &eb_pos_only);
	glGenBuffers(1, &eb_pos_normal_tex);
	//glGenBuffers(1, &eb_pos_normal_tex_tangent);


	//// POS_ONLY
	//glBindVertexArray(va_pos_only);
	//// 0. 复制顶点数组到缓冲中供OpenGL使用
	//glBindBuffer(GL_ARRAY_BUFFER, vb_pos_only);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_only);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//// 1. 设置顶点属性指针
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// POS_NORMAL_TEX
	glBindVertexArray(va_pos_normal_tex);
	glBindBuffer(GL_ARRAY_BUFFER, vb_pos_normal_tex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_normal_tex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//// vertex texture coords
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));


	//// POS_NORMAL_TEX_TANGENT
	//glBindVertexArray(va_pos_normal_tex_tangent);
	//glBindBuffer(GL_ARRAY_BUFFER, vb_pos_normal_tex_tangent);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_normal_tex_tangent);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

}

void MeshRenderer::RenderMesh()
{
	// draw our first triangle
	glUseProgram(default_shaderProgram);
	glBindVertexArray(va_pos_normal_tex);
	if (1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, 0);
}

void MeshRenderer::ComplieShaders()
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.open("VertexShader.glsl");
	fShaderFile.open("PixelShader.glsl");
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	fShaderFile.close();
	vShaderFile.close();
	std::string vertexCode = vShaderStream.str();
	std::string fragmentCode = fShaderStream.str();
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	default_shaderProgram = glCreateProgram();
	glAttachShader(default_shaderProgram, vertexShader);
	glAttachShader(default_shaderProgram, fragmentShader);
	glLinkProgram(default_shaderProgram);
	// check for linking errors
	glGetProgramiv(default_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(default_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void MeshRenderer::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(default_shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
