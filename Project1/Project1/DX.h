#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>
#include "DisplayClear.h"

class DirectX {
public:
	void dx(HWND hwnd);
	
	IDXGIFactory4* factory;
	ID3D12CommandQueue* cq;
	IDXGIAdapter* HWadapter;
	IDXGISwapChain3* sc;
	ID3D12Device* d3d12d;
};