#pragma once
#include <iostream>
#include "Engine/GameObject/GameObject.h"

using namespace std;

class Component {
public:
	string name;
	Component(string name, GameObject* parent);
	void PreRender();
	void Update();

	GameObject* parent;
};