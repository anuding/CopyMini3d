#include "CPMScene.h"



CPMScene::CPMScene()
{
}


CPMScene::~CPMScene()
{
}
bool CPMScene::AddGameObject(CPMGameObject* obj)
{
	obj_list.push_back(obj);
	return true;
}