#include "Engine/GameObject/GameObject.h"

GameObject::GameObject(string name) {
	this->transform = Transform{};
	this->name = name;
}

void GameObject::AddComponent(Component* component) {

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