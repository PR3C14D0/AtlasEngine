#include "Engine/GameObject/Component.h"

Component::Component(Transform* transform) {
	this->transform = transform;
}

void Component::PreRender() {
	
}

void Component::Update() {
	std::cout << this->transform->location.x << std::endl;
}

void Component::LoadModel() {

}