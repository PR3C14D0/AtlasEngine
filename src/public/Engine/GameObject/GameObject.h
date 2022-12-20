#pragma once
#include <iostream>
#include <vector>
#include "Math/Transform.h"
#include "Component.h"

using namespace std;

class GameObject {
private:
	vector<Component*> components;
public:
	string name;
	Transform transform;

	void AddComponent(Component* component);

	void PreRender();
	void Update();

	GameObject(string name);
};