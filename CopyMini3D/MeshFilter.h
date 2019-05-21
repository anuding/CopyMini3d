#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include "Geometry.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
namespace Engine::Components {
	struct Texture {
		unsigned int id;
		string type;
		string path;
	};
	class MeshFilter : public Component
	{
	public:
		MeshFilter();
		~MeshFilter();
		virtual void Update();
		void LoadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		string directory;
	};
}