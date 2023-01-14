#include "Math/Transform.h"

Transform::Transform() {
	this->location = Vector3{ 0.f, 0.f, 0.f };
	this->rotation = Vector3{ 0.f, 0.f, 0.f };
	this->scale = Vector3{ 0.f, 0.f, 0.f };
}

Transform::Transform(Vector3 v) {
	this->location = v;
	this->rotation = v;
	this->scale = v;
}

Transform::Transform(Vector3 location, Vector3 rotation, Vector3 scale) {
	this->location = location;
	this->rotation = rotation;
	this->scale = scale;
}

void Transform::translate(Vector3 xyz)
{
	this->location = this->location + xyz;
}

void Transform::translate(float x, float y, float z) {
	Vector3 added = Vector3{ x, y, z };
	this->location = this->location + added;
}

void Transform::rotate(Vector3 xyz)
{
	this->rotation = this->rotation + xyz;
}

void Transform::rotate(float x, float y, float z) {
	Vector3 added = Vector3{ x, y, z };
	this->rotation = this->rotation + added;
}

Vector3 Transform::forward() { //
	Vector3 forwardPoint = this->RotatePoint({0.f, 0.f, 1.f});
	return forwardPoint;
}

Vector3 Transform::right() {
	Vector3 rightPoint = this->RotatePoint({ 1.f, 0.f, 0.f });
	return rightPoint;
}

Vector3 Transform::RotatePoint(Vector3 point) {
	XMMATRIX rotation = XMMatrixTranspose(XMMatrixIdentity());
	rotation *= XMMatrixTranspose(XMMatrixRotationX(XMConvertToRadians(this->rotation.x)));
	rotation *= XMMatrixTranspose(XMMatrixRotationY(XMConvertToRadians(this->rotation.y)));
	rotation *= XMMatrixTranspose(XMMatrixRotationZ(XMConvertToRadians(this->rotation.z)));
	
	XMVECTOR XMRotToPoint = XMVector4Transform(XMVectorSet(point.x, point.y, point.z, 1.f), rotation);
	XMFLOAT4 loadedRotation;
	XMStoreFloat4(&loadedRotation, XMRotToPoint);
	Vector3 rotToPoint = Vector3{ loadedRotation.x, loadedRotation.y, loadedRotation.z };

	return rotToPoint;
}
