#pragma once
#include "Component.h"
#include "Geometry.h"
#include <vector>
#include <string>
namespace Engine::Components {
	
	class MeshRenderer : public Component
	{
		typedef unsigned int VAO;
		typedef unsigned int VBO;
		typedef unsigned int EBO;

	public:
		MeshRenderer();
		~MeshRenderer();
		virtual void Update();
		void RenderMesh();
		void Bind(std::vector<Vertex>& v, std::vector<unsigned int>& i);
		void ComplieShaders();
		int default_shaderProgram;

		VAO va_pos_only, va_pos_normal_tex, va_pos_normal_tex_tangent;
		VBO vb_pos_only, vb_pos_normal_tex, vb_pos_normal_tex_tangent;
		EBO eb_pos_only, eb_pos_normal_tex, eb_pos_normal_tex_tangent;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		int length;
	};
}


