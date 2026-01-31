#pragma once

#include "DX.h"

class CommandAllocator {
public:
	bool create(ID3D12Device& device, D3D12_COMMAND_LIST_TYPE type);

	void reset();

private:
	ID3D12CommandAllocator* commandAllocatorp;
	D3D12_COMMAND_LIST_TYPE type;
};