#pragma once
#include <Windows.h>
#include "Math/Vector3.h"
#include <DirectX/D3D11.h>
#include <DirectX/xnamath.h>

struct Transform {
private:
	Vector3 RotatePoint(Vector3 point);
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

	Vector3 forward();
	Vector3 right();
};