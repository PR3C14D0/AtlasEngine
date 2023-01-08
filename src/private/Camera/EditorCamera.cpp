#include "Engine/Camera/EditorCamera.h"

EditorCamera::EditorCamera(std::string name) : Camera(name) {
	this->input = Input::GetInstance();
	this->speed = -.5f;
	this->backwardSpeed = .5f;
	this->time = Time::GetInstance();
	this->hideCursor = false;
}

void EditorCamera::PreRender() {
	Camera::PreRender();
	this->transform.translate(0.f, 0.f, 2.f);
}

void EditorCamera::Update() {
	Camera::Update();

	this->input->HideCursor(hideCursor);

	if (this->input->GetMouseButtonDown(RIGHT_CLICK)) {
		this->hideCursor = true;
		float deltaX = this->input->GetAxis("Right");
		float deltaY = this->input->GetAxis("Up");

		this->transform.rotate(deltaY * time->deltaTime, deltaX * time->deltaTime, 0.f);
		if (this->input->GetKeyDown('w')) {
			this->transform.translate(transform.forward() * time->deltaTime * speed);
		}

		if (this->input->GetKeyDown('s')) {
			this->transform.translate(transform.forward() * time->deltaTime * backwardSpeed);
		}

		if (this->input->GetKeyDown('d')) {
			this->transform.translate(transform.right() * time->deltaTime * speed);
		}
		if (this->input->GetKeyDown('a')) {
			this->transform.translate(transform.right() * time->deltaTime * backwardSpeed);
		}
	}
	else {
		this->hideCursor = false;
	}
}