#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>

class shader {
private:

public:
	ID3DBlob* vertexShader_;
	ID3DBlob* pixelShader_;

	bool cretate(ID3D12Device &device);
};