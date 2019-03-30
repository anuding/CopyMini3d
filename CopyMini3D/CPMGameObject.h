#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "matrix.h"

struct Index
{
	unsigned int pos;
	unsigned int tex;
	unsigned int normal;
};

class CPMGameObject {
public:
	//Vector4 pos;
	float rotaX;
	float rotaY;
	float size =  3.0f / 100.0f;
	CPMGameObject();
	bool LoadModel();
	bool InitAsTriangle();
	bool InitFromObj(std::string filename);
	bool InitAsCube();


	std::vector<Vector3> pos_list;
	std::vector<Vector3> tex_list;
	std::vector<Vector3> normal_list;
	std::vector<Index> index_list;
private:

};