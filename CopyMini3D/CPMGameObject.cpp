#include "CPMGameObject.h"




CPMGameObject::CPMGameObject()
{
}

bool CPMGameObject::InitAsTriangle()
{
	//Vertex v1(-1, 1, 0), v2(1, 0, 0), v3(-1, 0, 0), v4(1, 1, 0);
	//vertex_list.push_back(v1);
	//vertex_list.push_back(v2);
	//vertex_list.push_back(v3);
	//vertex_list.push_back(v1);
	//vertex_list.push_back(v4);
	//vertex_list.push_back(v2);

	//index_list.push_back(0);
	//index_list.push_back(1);
	//index_list.push_back(2);
	//index_list.push_back(3);
	//index_list.push_back(4);
	//index_list.push_back(5);
	return true;
}

bool CPMGameObject::InitAsCube()
{
	Vertex mesh[8] = {
		{ { -1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
		{ {  1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
		{ {  1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
		{ { -1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
		{ { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
		{ {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
		{ {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
		{ { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	};
	for (int i = 0; i < 8; i++)
	{
		vertex_list.push_back(mesh[i]);
	}


	index_list = { 0,3,1, 1,3,2,
				   1,2,5, 5,2,6,
	4,5,7, 7,6,5,
	0,3,4, 3,7,4,
	
	3,2,6, 6,7,3,
	0,1,4, 4,5,1};

	return false;
}
