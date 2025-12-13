#include "shader.h"
#include <cassert>
#include <string>


class shader {
private:

public:
	bool create(ID3D12Device& device) {
		const std::string filePath0 = "VertexShader.hlsl";
		const std::string filePath1 = "PixelShader.hlsl";

		const std::wstring temp0 = std::wstring(filePath0.begin(), filePath0.end());
		const std::wstring temp1 = std::wstring(filePath1.begin(), filePath1.end());

        ID3DBlob* error{};

        auto res = D3DCompileFromFile(temp0.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader_, &error);
        if (FAILED(res)) {
            char* p = static_cast<char*>(error->GetBufferPointer());
            assert(false && "頂点シェーダのコンパイルに失敗しました");
        }
        res = D3DCompileFromFile(temp1.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader_, &error);
        if (FAILED(res)) {
            char* p = static_cast<char*>(error->GetBufferPointer());
            assert(false && "ピクセルシェーダのコンパイルに失敗しました");
        }

        if (error) {
            error->Release();
        }

        return true;

	}
};