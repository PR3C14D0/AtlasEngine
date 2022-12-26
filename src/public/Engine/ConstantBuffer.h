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

	ConstantBuffer();

	static ConstantBuffer* GetInstance();
};