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

	class DXGI {
	public:
		IDXGIFactory4* CreateDXGIFactory();
	};

	class adapter {
	public:
		IDXGIAdapter* GetHardwareAdapter(IDXGIFactory4* factory);
	};

	class commandQueue {
	public:
		ID3D12CommandQueue* CreateCommandQueue(ID3D12Device* device);
	};

	class device {
	public:
		ID3D12Device* CreateD3D12Device(IDXGIAdapter* adapter);
	};

	class swapChain {
	public:
		IDXGISwapChain3* CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd);
	};

	class debugLayer {
	public:
		void EnableDebugLayer();
	};

	DXGI dxgic;
	adapter adpc;
	commandQueue cqc;
	device devc;
	swapChain scc;
	debugLayer dlc;
};