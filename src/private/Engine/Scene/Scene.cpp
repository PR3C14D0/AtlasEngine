#include "Engine/Scene/Scene.h"

Scene::Scene(std::string name) {
	this->name = name;
	this->dbg = Debugger::GetInstance();
}

void Scene::PreRender() {
	for (GameObject* obj : this->objs) {
		obj->PreRender();
	}
}

void Scene::GetObjects(std::vector<GameObject*>* vector) {
	*vector = this->objs;
}

void Scene::Update() {
	for (GameObject* obj : this->objs) {
		obj->Update();
	}
}

Camera* Scene::GetActualCamera() {
	if (!this->actualCamera)
		this->dbg->Throw("[ERROR] No active camera found");

	return this->actualCamera;
}

GameObject* Scene::GetObject(std::string name) {
	GameObject* retObj = nullptr;
	for (GameObject* obj : this->objs) {
		if (obj->name == name) {
			retObj = obj;
			break;
		}
	}

	if (retObj == nullptr) {
		std::string message = "[ERROR] GameObject with name " + name + " not found at scene " + this->name;
		dbg->Throw(message);
	}

	return retObj;
}

void Scene::SetActualCamera(std::string name) {
	GameObject* goCamera = this->GetObject(name);
	Camera* camera = dynamic_cast<Camera*>(goCamera);

	if (camera) {
		this->dbg->Log("[DEBUG] Camera set.");
	}
	else {
		this->dbg->Throw("[ERROR] That object is not a camera");
	}
}

void Scene::ClearObjects() {
	for (GameObject* object : this->objs) {
		if (object->HasMesh()) {
			if (Component* component = object->GetComponent("Mesh")) {
				component->Cleanup();
			}
		}
	}
}

void Scene::AddObject(GameObject* object) {
	this->objs.push_back(object);
}