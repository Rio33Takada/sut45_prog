#include "pipeline_state_object.h"

bool pipelineStateObject::create(ID3D12Device& device, ID3DBlob& vertexShader, ID3DBlob& pixelShader, ID3D12RootSignature& rootSignature) {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

	// 基本設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
	{"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = &rootSignature;

	// シェーダ設定

	psoDesc.VS = { vertexShader.GetBufferPointer(), vertexShader.GetBufferSize() };
	psoDesc.PS = { pixelShader.GetBufferPointer(), pixelShader.GetBufferSize() };

	// レンダリング状態設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	psoDesc.RasterizerState = rasterizerDesc;

	D3D12_BLEND_DESC blendDesc{};
	psoDesc.BlendState = blendDesc;

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

	return true;
}