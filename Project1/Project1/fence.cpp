#include "fence.h"
#include <cassert>

bool Fence::create(ID3D12Device& device) {
	// フェンスの生成
	HRESULT hr = device.CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fencep));

	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗しました");
		return false;
	}

	waitGpuEvent = CreateEvent(nullptr, false, false, "WAIT_GPU");

	if (!waitGpuEvent) {
		assert(false && "GPU 同期用のイベントハンドルの作成に失敗しました");
		return false;
	}
	return true;
}

void Fence::wait(UINT64 fenceValue) {
	if (!fencep) {
		assert(false && "フェンスが未作成です");
		return;
	}

	// フェンスの値が指定された値に達するまで待機
	if (fencep->GetCompletedValue() < fenceValue) {
		// GPU がフェンス値に到達するまで待つ
		fencep->SetEventOnCompletion(fenceValue, waitGpuEvent);
		WaitForSingleObject(waitGpuEvent, INFINITE);
	}
}