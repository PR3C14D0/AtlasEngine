#pragma once
#include <iostream>
#include "Math/Transform.h"

using namespace std;

class Component {
public:
	Transform* transform;
	string name;
	Component(Transform* transform);
	void PreRender();
	void Update();
};