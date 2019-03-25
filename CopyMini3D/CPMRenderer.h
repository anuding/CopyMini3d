#pragma once
#include "CPMScene.h"
#include "CPMDevice.h"
#include <Windows.h>
class CPMRenderer
{
public:
	CPMRenderer(CPMScene &_scene, CPMDevice &_device);
	~CPMRenderer();


	CPMScene* scene;
	CPMDevice* device;

	void Draw();
};

//vertex_buffer.insert(vertex_buffer.end(),
//	obj->vertex_list.begin(), obj->vertex_list.end());
//index_buffer.insert(index_buffer.end(),
//	obj->index_list.begin(), obj->index_list.end());