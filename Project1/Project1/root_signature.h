#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>

class rootSignature {
private:

public:
	bool create(ID3D12Device& device);
	
	ID3D12RootSignature* rootSignature;
};