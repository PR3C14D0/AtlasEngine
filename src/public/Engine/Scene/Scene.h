#pragma once
#include "Engine/GameObject/GameObject.h"

class Scene {
	friend class SceneManager;
private:
	std::string name;
	std::vector<GameObject*> objs;
public:
	Scene() = default;
	Scene(std::string name);

	void PreRender();
	void Update();
};