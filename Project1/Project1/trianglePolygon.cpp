#include "trianglePolygon.h"
#include <DirectXMath.h>

struct  Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

class trianglePolygon {
private:
	bool createVertexBuffer(ID3D12Device& device) {
		Vertex triangleVertices[] =
		{
			// 上頂点（赤色）
			{ {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			// 右下頂点（緑色）
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			// 左下頂点（青色）
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		// 1. 「CPUからデータを送り込めるメモリを使いたい」
		D3D12_HEAP_PROPERTIES heapProperty{};
		heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperty.CreationNodeMask = 1;
		heapProperty.VisibleNodeMask = 1;

		// 2. 「頂点データ分のサイズの1次元バッファがほしい」
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = sizeof(triangleVertices);
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// 3. 上記の条件でリソースを作成
		ID3D12Resource* vertexBuffer;
		device.CreateCommittedResource(
			&heapProperty,                      // どこのメモリを使うか
			D3D12_HEAP_FLAG_NONE,               // ヒープフラグ
			&resourceDesc,                      // どんなリソースを作るか
			D3D12_RESOURCE_STATE_GENERIC_READ,  // 初期状態
			nullptr,                            // クリア値
			IID_PPV_ARGS(&vertexBuffer)         // 作成されるリソース
		);

		// 頂点バッファにデータを転送する
		// CPU からアクセス可能なアドレスを取得
		Vertex* data{};

		// バッファをマップ（CPUから更新開始）
		// vertexBuffer を直接利用するのではなく、data を介して更新するイメージ
		vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

		// 頂点データをコピー
		memcpy(data, triangleVertices, sizeof(triangleVertices));

		// コピーが終わったのでマップ解除（CPUから更新終了）
		// ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleVertices は不要になる
		vertexBuffer->Unmap(0, nullptr);

		// 描画の際に利用するビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // GPU から見た時のデータ格納アドレス（GPU 仮想アドレス）
		vertexBufferView.StrideInBytes = sizeof(Vertex);                        // 1頂点のサイズ
		vertexBufferView.SizeInBytes = sizeof(triangleVertices);                // 全体のサイズ
	}

	bool createIndexBuffer(ID3D12Device& device) {
		// インデックスデータ（16bit符号なし整数）
		UINT16 triangleIndices[] = { 0, 1, 2 };

		// 1. 「CPUからデータを送り込めるメモリを使いたい」
		D3D12_HEAP_PROPERTIES heapProperty{};
		heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperty.CreationNodeMask = 1;
		heapProperty.VisibleNodeMask = 1;

		// 2. 「頂点データ分のサイズの1次元バッファがほしい」
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = sizeof(triangleIndices);
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// 3. 上記の条件でリソースを作成
		ID3D12Resource* vertexBuffer;
		device.CreateCommittedResource(
			&heapProperty,                      // どこのメモリを使うか
			D3D12_HEAP_FLAG_NONE,               // ヒープフラグ
			&resourceDesc,                      // どんなリソースを作るか
			D3D12_RESOURCE_STATE_GENERIC_READ,  // 初期状態
			nullptr,                            // クリア値
			IID_PPV_ARGS(&vertexBuffer)         // 作成されるリソース
		);

		// 頂点バッファにデータを転送する
// CPU からアクセス可能なアドレスを取得
		Vertex* data{};

		// バッファをマップ（CPUから更新開始）
		// vertexBuffer を直接利用するのではなく、data を介して更新するイメージ
		vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

		// 頂点データをコピー
		memcpy(data, triangleIndices, sizeof(triangleIndices));

		// コピーが終わったのでマップ解除（CPUから更新終了）
		// ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleVertices は不要になる
		vertexBuffer->Unmap(0, nullptr);

		// 描画の際に利用するビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // GPU から見た時のデータ格納アドレス（GPU 仮想アドレス）
		vertexBufferView.StrideInBytes = sizeof(Vertex);                        // 1頂点のサイズ
		vertexBufferView.SizeInBytes = sizeof(triangleIndices);                // 全体のサイズ
	}


public:
	bool create(ID3D12Device& device) {
		createVertexBuffer(device);
		createIndexBuffer(device);
	}

	void draw() {

	}
};