#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectX/xnamath.h>
#include "Engine/ConstantBuffer.h"
#include "Engine/GameObject/GameObject.h"

class Camera : public GameObject {
private:
	ConstantBuffer* MVP;
public:
	explicit Camera(std::string name);

	void PreRender() override;
	void Update() override;
};