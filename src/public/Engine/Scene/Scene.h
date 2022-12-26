#pragma once
#include <iostream>
#include "Engine/GameObject/GameObject.h"
#include "Engine/Debugger.h"

class Scene {
	friend class SceneManager;
private:
	std::string name;
	std::vector<GameObject*> objs;

	Debugger* dbg;
public:
	Scene() = default;
	Scene(std::string name);

	void PreRender();
	void Update();

	GameObject* GetObject(std::string name);
	
	void GetObjects(std::vector<GameObject*>* vector);
	void AddObject(GameObject* object);

	void ClearObjects();
};