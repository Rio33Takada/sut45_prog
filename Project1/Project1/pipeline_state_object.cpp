#include "pipeline_state_object.h"

class pipelineStateObject {
private:

public:
	bool create(ID3D12Device& device, ID3DBlob& vertexShader, ID3DBlob& pixelShader, ID3D12RootSignature& rootSignature) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

		// 基本設定
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = &rootSignature;

		// シェーダ設定
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);

		// レンダリング状態設定
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// 深度・ステンシル設定（今回は無効）
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;

		// その他設定
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		// PSO作成
		ID3D12PipelineState* pipelineState;
		device.CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	}
};