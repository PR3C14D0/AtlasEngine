#include "Engine/Shader.h"
#include "Engine/Core.h"

Shader::Shader(std::string path, std::string sVertexShaderFuncName, std::string sPixelShaderFuncName, const D3D11_INPUT_ELEMENT_DESC* iLayoutDesc, UINT nNumElements) {
	this->core = Core::GetInstance(); // Get our core singleton
	this->dbg = Debugger::GetInstance(); // Get our debugger singleton

	this->core->GetDevice(&this->dev, &this->con); // Get our Device and Inmediate Context
	
	ComPtr<ID3DBlob> VertexShader;
	ComPtr<ID3DBlob> PixelShader;

	ComPtr<ID3DBlob> VertexShaderErrors;
	ComPtr<ID3DBlob> PixelShaderErrors;
	
	/* Compilation of our shader */
	D3DX11CompileFromFile(path.c_str(), nullptr, NULL, sVertexShaderFuncName.c_str(), "vs_5_0", NULL, NULL, nullptr, VertexShader.GetAddressOf(), VertexShaderErrors.GetAddressOf(), nullptr);
	D3DX11CompileFromFile(path.c_str(), nullptr, NULL, sPixelShaderFuncName.c_str(), "ps_5_0", NULL, NULL, nullptr, PixelShader.GetAddressOf(), PixelShaderErrors.GetAddressOf(), nullptr);

	/* Check if there are errors. If there are, delete our this instance and return */
	if (VertexShaderErrors|| PixelShaderErrors) {
		if (VertexShaderErrors) {
			dbg->Throw((char*)VertexShaderErrors->GetBufferPointer());
			VertexShaderErrors->Release();
		}
		if (PixelShaderErrors) {
			dbg->Throw((char*)PixelShaderErrors->GetBufferPointer());
			PixelShaderErrors->Release();
		}

		delete this;
		return;
	}
	
	/* Create our Vertex and Pixel shaders */
	this->dev->CreateVertexShader(VertexShader->GetBufferPointer(), VertexShader->GetBufferSize(), nullptr, this->VShader.GetAddressOf());
	this->dev->CreatePixelShader(PixelShader->GetBufferPointer(), PixelShader->GetBufferSize(), nullptr, this->PShader.GetAddressOf());

	/* Create our input layout*/
	this->dev->CreateInputLayout(iLayoutDesc, nNumElements, VertexShader->GetBufferPointer(), VertexShader->GetBufferSize(), this->iLayout.GetAddressOf());

	/* Release both Vertex and Pixel shader blob */
	if (VertexShader)
		VertexShader->Release();
	if (PixelShader)
		PixelShader->Release();
}

Shader::~Shader() {
	/* Our class destructor. We release all the class' ComPtrs */
	if (this->VShader)
		this->VShader->Release();
	if (this->PShader)
		this->PShader->Release();
	if (this->iLayout)
		this->iLayout->Release();
}

void Shader::UseShader() {
	/* We tell to our Inmediate context that we'll use our Vertex and Pixel shaders and our Input layout */
	this->con->VSSetShader(this->VShader.Get(), nullptr, NULL);
	this->con->PSSetShader(this->PShader.Get(), nullptr, NULL);
	this->con->IASetInputLayout(this->iLayout.Get());

	return;
}