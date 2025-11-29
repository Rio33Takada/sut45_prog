#include "DisplayClear.h"

class DescriptorHeap{
public:
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	ID3D12DescriptorHeap* rtvHeap;

	void create(ID3D12Device& device) {
		// RTVディスクリプタヒープの作成例
		//D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;  // スワップチェーンのバッファ数
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		//ID3D12DescriptorHeap* rtvHeap;
		device.CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	}
};

class RenderTarget {
public:
	void create(IDXGISwapChain3& swapChain, ID3D12Device& device, DescriptorHeap& heap) {
		// レンダーターゲット作成例
		ID3D12Resource* renderTargets[2];  // スワップチェーンでバッファが二つあるのでダブルバッファで

		D3D12_DESCRIPTOR_HEAP_TYPE heapType = heap.rtvHeapDesc.Type;

		for (int i = 0; i < 2; i++)
		{
			// スワップチェーンからバッファを取得
			swapChain.GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

			// RTVハンドルの計算
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = heap.rtvHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHandle.ptr += device.GetDescriptorHandleIncrementSize(heapType); // i * は必要？.

			// RTVの作成
			device.CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
		}
	}
};

class CommandAllocator {
public:
	ID3D12CommandAllocator* commandAllocators[2];

	void test(ID3D12Device& device) {
		// コマンドアロケータ作成例
		//ID3D12CommandAllocator* commandAllocators[2];

		for (int i = 0; i < 2; i++)
		{
			device.CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&commandAllocators[i])
			);
		}
	}
};

class CommandList {
public:
	void test(ID3D12Device& device, CommandAllocator& commandAllocator, IDXGISwapChain3* swapChain) {
		// コマンドリスト作成例
		ID3D12GraphicsCommandList* commandList;
		device.CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandAllocator.commandAllocators[swapChain->GetCurrentBackBufferIndex()],
			nullptr,
			IID_PPV_ARGS(&commandList)
		);

		// 初期状態でクローズしておく
		commandList->Close();
	}
};

class Fence {
public:
	void test(ID3D12Device* device) {
		// フェンスの生成例
		ID3D12Fence* fence;
		UINT64 fenceValue = 0;
		HANDLE fenceEvent;

		// フェンスの生成
		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		// フェンスイベントの生成
		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
};

void DisplayClear:: displayClear() {

}