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
