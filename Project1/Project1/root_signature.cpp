#include "root_signature.h"
#include <cassert>

bool rootSignature::create(ID3D12Device &device) {
    // 描画に必要なリソースをシェーダに伝える
    // 今回は特にリソースを利用しないので空で用意
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // ルートシグネチャをシリアライズ
    ID3DBlob* signature;
    auto      res = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signature,
        nullptr);

    bool success = SUCCEEDED(res);
    if (!success) {
        assert(false && "ルートシグネチャのシリアライズに失敗");
    }
    else {
        // ルートシグネチャの生成
        res = device.CreateRootSignature(
            0,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature));

        success &= SUCCEEDED(res);
        if (!success) {
            assert(false && "ルートシグネチャの生成に失敗");
        }
    }

    if (signature) {
        signature->Release();
    }

    return success;
}

    // ルートシグネチャオブジェクトを作成
    //ID3D12RootSignature* rootSignature;
    //device.CreateRootSignature(
    //    0,                              // ノードマスク（GPUが複数ある時などに必要な指定）
    //    signature->GetBufferPointer(),  // シリアライズデータ
    //    signature->GetBufferSize(),     // データサイズ
    //    IID_PPV_ARGS(&rootSignature)    // 作成されるオブジェクト
    //);

    //return true;
//}