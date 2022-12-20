#pragma once
#include "Math/Vector3.h"

struct Transform {
public:
	Vector3 location;
	Vector3 rotation;
	Vector3 scale;

	Transform();
	Transform(Vector3 v);
	Transform(Vector3 location, Vector3 rotation, Vector3 scale);

	void translate(Vector3 xyz);
	void translate(float x, float y, float z);

	void rotate(Vector3 xyz);
	void rotate(float x, float y, float z);
};