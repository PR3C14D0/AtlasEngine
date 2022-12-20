#pragma once
#include <iostream>
#include <vector>
#include "Math/Transform.h"
#include "Component.h"

class GameObject {
private:
	std::vector<Component*> components;
public:
	std::string name;
	Transform transform;

	void AddComponent(Component* component);

	void PreRender();
	void Update();

	GameObject(std::string name);
};