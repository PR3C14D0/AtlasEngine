#pragma once
#include <iostream>
#include "Engine/GameObject/GameObject.h"
#include "Engine/Debugger.h"
#include "Engine/Camera/Camera.h"

class Scene {
	friend class SceneManager;
private:
	std::string name;
	std::vector<GameObject*> objs;

	Debugger* dbg;

	Camera* actualCamera;
public:
	Scene() = default;
	Scene(std::string name);

	void PreRender();
	void Update();

	GameObject* GetObject(std::string name);

	Camera* GetActualCamera();
	
	void GetObjects(std::vector<GameObject*>* vector);
	void AddObject(GameObject* object);
	void SetActualCamera(std::string name);

	void ClearObjects();
};