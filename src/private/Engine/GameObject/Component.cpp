#include "Engine/GameObject/Component.h"

Component::Component(Transform* transform) {
	this->transform = transform;
}

void Component::PreRender() {
	
}

void Component::Update() {
	cout << this->transform->location.x << endl;
}