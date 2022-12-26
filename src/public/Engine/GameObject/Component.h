#pragma once
#include <iostream>
#include "Math/Transform.h"

class Component {
public:
	Transform* transform;
	std::string name;
	Component(Transform* transform);
	virtual void PreRender();
	virtual void Update();
	virtual void LoadModel(std::string name);
	virtual void Cleanup();
};