#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>
#include "DX.h"

class DisplayClear {
public:
	void displayClear();


};

class DescriptorHeap {
public:
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	ID3D12DescriptorHeap* rtvHeap;

	void create(ID3D12Device& device);
};

class CommandAllocator {
public:
	ID3D12CommandAllocator* commandAllocators[2];

	void test(ID3D12Device& device);
};

class RenderTarget {
public:
	void create(IDXGISwapChain3& swapChain, ID3D12Device& device, DescriptorHeap& heap);
};

class CommandList {
public:
	void test(ID3D12Device& device, CommandAllocator& commandAllocator, IDXGISwapChain3* swapChain);
};

//class Fence {
//public:
//	void test(ID3D12Device* device);
//};