#include "Engine/Scene/Scene.h"

Scene::Scene(string name) {
	this->name = name;
}

void Scene::PreRender() {
	for (GameObject* obj : this->objs) {
		obj->PreRender();
	}
}

void Scene::Update() {
	for (GameObject* obj : this->objs) {
		obj->Update();
	}
}