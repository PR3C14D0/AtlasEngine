#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectX/xnamath.h>
#include "Engine/Camera/Camera.h"
#include "Engine/Input.h"
#include "Module/Time.h"

class EditorCamera : public Camera {
private:
	Input* input;
	Time* time;

	float speed;
	float backwardSpeed;
	bool hideCursor;
public:
	EditorCamera(std::string name);

	void PreRender() override;
	void Update() override;
};