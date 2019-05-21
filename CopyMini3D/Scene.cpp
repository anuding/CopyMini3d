#include "Scene.h"
using namespace Engine::Scene;
void Scene::AddGameObject(GameObject* obj)
{
	obj_list.push_back(obj);
}

Scene::Scene()
{
}


Scene::~Scene()
{
}
