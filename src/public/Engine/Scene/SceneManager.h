#pragma once
#include <iostream>
#include <map>
#include "Engine/Debugger.h"
#include "Engine/Scene/Scene.h"
#include "Engine/GameObject/Component.h"
#include "Engine/GameObject/Components/Mesh.h"
#include "Engine/ConstantBuffer.h"
#include "Engine/Camera/Camera.h"

class Core; /* Forward declaration of our core. To avoid circular includes */

class SceneManager {
private:
	Debugger* dbg;
	std::map<std::string, Scene*> scenes;

	Core* core;

	Scene* actualScene;
public:
	void AddScene(Scene* scene);
	void LoadScene(std::string name);

	void Update();

	SceneManager();

	Scene* GetActualScene();
};