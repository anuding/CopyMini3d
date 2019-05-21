#include "GameObject.h"
using namespace Engine::GameObjects;
using namespace Engine::Components;
GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::AddCompoent(Component* compoent)
{
	compoent_list.push_back(compoent);
}
