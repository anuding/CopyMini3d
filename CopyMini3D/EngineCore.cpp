#include "EngineCore.h"
#include "GLHeader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CPM::Engine
{
	EngineCore::EngineCore()
	{
		ComplieShaders();
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			//1, 2, 3   // second Triangle
		};
		
		
		glGenVertexArrays(1, &va_pos_only);
		glGenVertexArrays(1, &va_pos_normal_tex);
		glGenVertexArrays(1, &va_pos_normal_tex_tangent);

		glGenBuffers(1, &vb_pos_only);
		glGenBuffers(1, &vb_pos_normal_tex);
		glGenBuffers(1, &vb_pos_normal_tex_tangent);

		glGenBuffers(1, &eb_pos_only);
		glGenBuffers(1, &eb_pos_normal_tex);
		glGenBuffers(1, &eb_pos_normal_tex_tangent);


		// POS_ONLY
		glBindVertexArray(va_pos_only);
			// 0. 复制顶点数组到缓冲中供OpenGL使用
			glBindBuffer(GL_ARRAY_BUFFER, vb_pos_only);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_only);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// 1. 设置顶点属性指针
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

		// POS_NORMAL_TEX
		glBindVertexArray(va_pos_normal_tex);
			glBindBuffer(GL_ARRAY_BUFFER, vb_pos_normal_tex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_normal_tex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

		// POS_NORMAL_TEX_TANGENT
		glBindVertexArray(va_pos_normal_tex_tangent);
			glBindBuffer(GL_ARRAY_BUFFER, vb_pos_normal_tex_tangent);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_pos_normal_tex_tangent);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
	}


	EngineCore::~EngineCore()
	{
	}

	void EngineCore::Draw()
	{
		// draw our first triangle
		glUseProgram(default_shaderProgram);
		glBindVertexArray(va_pos_only);
		if(1)
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}

	void EngineCore::ComplieShaders()
	{
		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
}

