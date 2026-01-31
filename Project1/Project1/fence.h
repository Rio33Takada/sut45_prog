#pragma once

#include "DX.h"

class Fence {
public:
	bool create(ID3D12Device& device);

	void wait(UINT64 fenceValue);
private:
	ID3D12Fence* fencep;

	HANDLE waitGpuEvent;
};