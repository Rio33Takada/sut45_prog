#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>

class trianglePolygon {
private:
	bool createVertexBuffer(ID3D12Device& device);

	bool createIndexBuffer(ID3D12Device& device);
public:
	bool create(ID3D12Device& device);
};