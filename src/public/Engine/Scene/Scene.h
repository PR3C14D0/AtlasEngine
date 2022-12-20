#pragma once
#include "Engine/GameObject/GameObject.h"

using namespace std;

class Scene {
	friend class SceneManager;
private:
	string name;
	vector<GameObject*> objs;
public:
	Scene() = default;
	Scene(string name);

	void PreRender();
	void Update();
};