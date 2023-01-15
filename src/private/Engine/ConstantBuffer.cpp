#include "Engine/ConstantBuffer.h"

ConstantBuffer* ConstantBuffer::instance;

ConstantBuffer::ConstantBuffer() {
	this->View = XMMatrixTranspose(XMMatrixLookToLH(XMVectorSet(0.f, 0.f, -2.f, 0.f), XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	this->LightPos = XMFLOAT4(0.f, 10.f, -10.f, 1.f); 
	this->AmbientColor = XMFLOAT4(.2f, .2f, .2f, 1.f);
}

ConstantBuffer* ConstantBuffer::GetInstance() {
	if (ConstantBuffer::instance == nullptr)
		ConstantBuffer::instance = new ConstantBuffer();
	return ConstantBuffer::instance;
}