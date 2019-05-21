#pragma once
namespace Engine::Components {
	class Component
	{
	public:
		Component();
		~Component();

		virtual void Update() = 0;
	};
}
