#include "MeshFilter.h"

#include <iostream>

using namespace Engine::Components;
void MeshFilter::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void MeshFilter::processNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
void MeshFilter::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		if (mesh->mNormals == NULL)
		{
			vertex.normal.x = 0;
			vertex.normal.y = 0;
			vertex.normal.z = 0;
		}
		else
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		if (mesh->mTextureCoords[0])
		{
			vertex.texcoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

MeshFilter::MeshFilter()
{
	LoadModel("../res/sphere0_3cm.obj");//sphere0_3cm
}

MeshFilter::~MeshFilter()
{
}

void MeshFilter::Update()
{
	return;
}
