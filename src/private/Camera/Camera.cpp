#include "Engine/Camera/Camera.h"

Camera::Camera(std::string name) : GameObject(name) {
	this->MVP = ConstantBuffer::GetInstance();
}

void Camera::Update() {
	this->MVP->View = XMMatrixTranspose(XMMatrixLookToLH(XMVectorSet(0.f, 0.f, -2.f, 0.f), XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
}