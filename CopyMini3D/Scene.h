#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <vector>
namespace Engine::Scene {
	using namespace Engine::GameObjects;
	class Scene : public Singleton
	{
	public:
		static const int OBJ_COUNT = 100;
		std::vector<GameObject*> obj_list;
	public:
		void AddGameObject(GameObject* obj);
		
		Scene();
		~Scene();
	};
}