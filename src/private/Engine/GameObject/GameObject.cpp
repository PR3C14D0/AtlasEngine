#include "Engine/GameObject/GameObject.h"

GameObject::GameObject(std::string name) {
	this->transform = Transform{ {1.f, 0.f, 0.f} };
	this->name = name;
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

void GameObject::Update() {
	for (Component* component : this->components) {
		component->Update();
	}
}