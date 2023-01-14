#pragma once
#include <Windows.h>
#include <DirectX/xnamath.h>

struct ConstantBuffer {
private:
	static ConstantBuffer* instance;
public:
	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Projection;
	XMFLOAT4 LightPos;
	XMFLOAT4 AmbientColor;

	ConstantBuffer();

	static ConstantBuffer* GetInstance();
};