#include "Engine/Camera/Camera.h"

Camera::Camera(std::string name) : GameObject(name) {
	this->MVP = ConstantBuffer::GetInstance();
}

void Camera::PreRender() {
	this->transform.translate(0.f, 0.f, 2.f);
}

void Camera::Update() {
	/* TODO: Remove this. Is only testing. This must be at EditorCamera future class */
	this->MVP->View = XMMatrixTranspose(XMMatrixIdentity());
	this->MVP->View *= XMMatrixTranspose(XMMatrixRotationY(XMConvertToRadians(this->transform.rotation.y)));
	this->MVP->View *= XMMatrixTranspose(XMMatrixRotationX(XMConvertToRadians(this->transform.rotation.x)));
	this->MVP->View *= XMMatrixTranspose(XMMatrixRotationZ(XMConvertToRadians(this->transform.rotation.z)));
	this->MVP->View *= XMMatrixTranspose(XMMatrixTranslation(this->transform.location.x, this->transform.location.y, this->transform.location.z));
}