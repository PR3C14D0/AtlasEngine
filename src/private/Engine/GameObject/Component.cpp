#include "Engine/GameObject/Component.h"

Component::Component(string name, GameObject* parent) {
	this->name = name;
	this->parent = parent;
}

void Component::PreRender() {
	
}

void Component::Update() {

}