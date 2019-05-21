#pragma once
#include "Component.h"
#include <vector>
namespace Engine::GameObjects {
	class GameObject
	{
	public:
		std::vector<Engine::Components::Component*> compoent_list;

	public:
		GameObject();
		~GameObject();
		void AddCompoent(Engine::Components::Component* compoent);
	};
}
