#include "Engine/ConstantBuffer.h"

ConstantBuffer* ConstantBuffer::instance;

ConstantBuffer::ConstantBuffer() {
	this->View = XMMatrixTranspose(XMMatrixLookToLH(XMVectorSet(0.f, 0.f, -2.f, 0.f), XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
}

ConstantBuffer* ConstantBuffer::GetInstance() {
	if (ConstantBuffer::instance == nullptr)
		ConstantBuffer::instance = new ConstantBuffer();
	return ConstantBuffer::instance;
}