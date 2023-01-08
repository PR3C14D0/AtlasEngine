#include "Engine/GameObject/GameObject.h"

GameObject::GameObject(std::string name) {
	this->transform = Transform{ {0.f, 0.f, 0.f} };
	this->name = name;
	this->dbg = Debugger::GetInstance();
}

void GameObject::AddComponent(Component* component) {
	this->components.push_back(component);
}

void GameObject::PreRender() {
	for (Component* component : this->components) {
		component->PreRender();
	}
	return;
}

bool GameObject::HasMesh() {
	bool result = false;
	for (Component* component : this->components) {
		if (component->name == "Mesh") {
			result = true;
			break;
		}
	}

	return result;
}

Component* GameObject::GetComponent(std::string name) {
	Component* retComponent = nullptr;
	for (Component* component : this->components) {
		if (component->name == name) {
			retComponent = component;
			break;
		}
	}

	if (retComponent == nullptr) {
		std::string message = "[ERROR] Component " + name + " not found at GameObject " + this->name;
		this->dbg->Throw(message);
	}

	return retComponent;
}

void GameObject::Update() {
	for (Component* component : this->components) {
		component->Update();
	}
	this->transform.rotate(0.f, 1.f, 0.f);
}