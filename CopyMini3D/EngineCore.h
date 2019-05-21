#pragma once

namespace CPM::Engine
{
	typedef unsigned int VAO;
	typedef unsigned int VBO;
	typedef unsigned int EBO;
	//无情的绘图机器
	class EngineCore
	{
	public:
		void Update();
		void Draw();
		void ComplieShaders();
		EngineCore();
		~EngineCore();


		int default_shaderProgram;
		VAO va_pos_only, va_pos_normal_tex, va_pos_normal_tex_tangent;
		VBO vb_pos_only, vb_pos_normal_tex, vb_pos_normal_tex_tangent;
		EBO eb_pos_only, eb_pos_normal_tex, eb_pos_normal_tex_tangent;
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"}\n\0";
	};

}

