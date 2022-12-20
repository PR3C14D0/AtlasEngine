#pragma once
#include <iostream>
#include <map>
#include "Engine/Error.h"
#include "Engine/Scene/Scene.h"
#include "Engine/GameObject/Components/Mesh.h"

class SceneManager {
private:
	Error* error;
	std::map<std::string, Scene*> scenes;

	Scene* actualScene;
public:
	void AddScene(Scene* scene);
	void LoadScene(std::string name);

	void Update();

	SceneManager();
};