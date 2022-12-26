#include "Engine/GameObject/Component.h"

Component::Component(Transform* transform) {
	this->transform = transform;
}

void Component::PreRender() {
	
}

void Component::Update() {
	
}

void Component::LoadModel(std::string name) {

}

void Component::Cleanup() {

}