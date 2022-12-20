#include "Engine/Scene/SceneManager.h"

SceneManager::SceneManager() {
	this->error = Error::GetInstance();
	this->actualScene = new Scene("asd");
}

void SceneManager::AddScene(Scene* scene) {
	this->scenes[scene->name] = scene;
}

void SceneManager::LoadScene(string name) {
	if (this->scenes.count(name) <= 0) {
		error->Throw("[ERROR] Scene not found.");
		cout << "[ERROR] Scene not found." << endl;
		return;
	}
	this->actualScene = this->scenes[name];
	return;
}

void SceneManager::Update() {
	this->actualScene->Update();
}