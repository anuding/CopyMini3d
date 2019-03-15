#pragma once
#include <vector>
//768KB per GameObject
struct Vertex {
	float x, y, z;
	Vertex() : x(0), y(0), z(0) {}
	Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};
class CPMGameObject {
public:
	CPMGameObject();
	bool LoadModel();
	bool InitAsTriangle();
	std::vector<Vertex> vertex_list;
	std::vector<int> index_list;
private:

};