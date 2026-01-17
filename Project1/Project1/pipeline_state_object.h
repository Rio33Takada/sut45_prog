#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>

class pipelineStateObject {
private:

public:
	bool create(ID3D12Device& device, ID3DBlob& vertexShader, ID3DBlob& pixelShader, ID3D12RootSignature& rootSignature);
};