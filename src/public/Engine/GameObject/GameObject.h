#pragma once
#include <iostream>
#include <vector>
#include "Math/Transform.h"
#include "Component.h"
#include "Engine/Debugger.h"

class GameObject {
private:
	std::vector<Component*> components;

	Debugger* dbg;
public:
	std::string name;
	Transform transform;

	void AddComponent(Component* component);
	Component* GetComponent(std::string name);

	virtual void PreRender();
	virtual void Update();
	virtual bool HasMesh();

	GameObject(std::string name);
};