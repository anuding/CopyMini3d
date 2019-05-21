#pragma once
#include "CPMGameObject.h"
#include "CPMLight.h"
#include "CPMCamera.h"
class CPMScene
{
public:
	CPMScene();
	~CPMScene();
	bool AddGameObject(CPMGameObject* obj);
	std::vector<CPMGameObject*> obj_list;
	std::vector<CPMLight*> light_list;
	std::vector<CPMCamera*> camera_list;
};

