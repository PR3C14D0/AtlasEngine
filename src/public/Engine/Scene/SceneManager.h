#pragma once
#include <iostream>
#include <map>
#include "Engine/Error.h"
#include "Engine/Scene/Scene.h"

using namespace std;

class SceneManager {
private:
	Error* error;
	map<string, Scene*> scenes;

	Scene* actualScene;
public:
	void AddScene(Scene* scene);
	void LoadScene(string name);

	void Update();

	SceneManager();
};