#pragma once
#include <vector>
#include "matrix.h"

class CPMGameObject {
public:
	//Vector4 pos;
	float rotaX;
	float rotaY;
	float size = 3.0f;
	CPMGameObject();
	bool LoadModel();
	bool InitAsTriangle();
	bool InitAsCube();
	std::vector<Vertex> vertex_list;
	std::vector<int> index_list;
private:

};