#include "CPMGameObject.h"

CPMGameObject::CPMGameObject()
{
}

bool CPMGameObject::InitAsTriangle()
{
	Vertex v1(1, 0, 0),v2(-1,0,0),v3(0,1,0);
	vertex_list.push_back(v1);
	vertex_list.push_back(v2);
	vertex_list.push_back(v3);

	index_list.push_back(0);
	index_list.push_back(1);
	index_list.push_back(2);
	return true;
}
