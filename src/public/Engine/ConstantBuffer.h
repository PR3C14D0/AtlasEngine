#pragma once
#include <DirectX/xnamath.h>

struct ConstantBuffer {
	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Projection;
};