#include "Math/Vector3.h"

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator+(Vector3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	return Vector3{ this->x + x, this->y + y, this->z + z };
}

Vector3 Vector3::operator-(Vector3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	return Vector3{ this->x - x, this->y - y, this->z - z };
}

Vector3 Vector3::operator*(float& f) {
	return Vector3{ this->x * f, this->y * f, this->z * f };
}