#include "Engine/Scene/SceneManager.h"

SceneManager::SceneManager() {
	this->error = Error::GetInstance();
	this->actualScene = new Scene("SampleScene");
	GameObject* sampleObj = new GameObject("TestObj");
	sampleObj->AddComponent(new Mesh(&sampleObj->transform));
	this->actualScene->objs.push_back(sampleObj);
	this->actualScene->PreRender();
}

void SceneManager::AddScene(Scene* scene) {
	this->scenes[scene->name] = scene;
}

void SceneManager::LoadScene(std::string name) {
	if (this->scenes.count(name) <= 0) {
		error->Throw("[ERROR] Scene not found.");
		std::cout << "[ERROR] Scene not found." << std::endl;
		return;
	}
	this->actualScene = this->scenes[name];
	return;
}

void SceneManager::Update() {
	this->actualScene->Update();
}