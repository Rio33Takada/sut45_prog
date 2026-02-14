#include <Windows.h>
#include "DX.h"
#include "DisplayClear.h"
#include "pipeline_state_object.h"
#include "root_signature.h"
#include "trianglePolygon.h"
#include "shader.h"
#include "fence.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // ×ボタンが押された時
        PostQuitMessage(0);  // 「アプリを終了します」とWindowsに通知
        return 0;
    case WM_PAINT:    // ウィンドウの再描画が必要な時
        // 画面を更新する処理をここに書く
        return 0;
    case WM_KEYDOWN:  // キーが押された時
        // キー入力の処理をここに書く
        return 0;
    }
    // 自分で処理しないメッセージはWindowsに任せる
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void RegisterWindowClass(HINSTANCE hInstance) 
{
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;           // ウィンドウプロシージャを指定（後述）
    wc.hInstance = hInstance;              // アプリケーションインスタンス
    wc.lpszClassName = "GameWindow";      // ウィンドウクラス名
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // マウスカーソル
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // 背景色

    RegisterClass(&wc);  // Windowsに登録
}

HWND FCreateWindow(HINSTANCE hInstance, int nCmdShow) 
{
    HWND hwnd = CreateWindow(
        "GameWindow",        // ウィンドウクラス名
        "My Game",           // ウィンドウタイトル
        WS_OVERLAPPEDWINDOW,  // ウィンドウスタイル
        CW_USEDEFAULT, CW_USEDEFAULT,  // 位置（自動）
        800, 600,            // サイズ（幅、高さ）
        NULL, NULL,          // 親ウィンドウ、メニュー
        hInstance,           // インスタンス
        NULL                 // 追加データ
    );

    ShowWindow(hwnd, nCmdShow);  // ウィンドウを表示
    return hwnd;
}

bool MessageLoop()
{
    // 3. メッセージループ
    MSG msg{};  // メッセージを格納する構造体
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // メッセージが来るまで待機
    {
        if (msg.message == WM_QUIT) return false; // 終了メッセージを受け取ったらfalseを返す.

        TranslateMessage(&msg);  // キーボードメッセージを使いやすい形に変換
        DispatchMessage(&msg);   // 適切なウィンドウプロシージャに送信
    }
    return true;
}

void MainGameLoop(DirectX& directX, UINT64 frameFenceValue_[], UINT64& nextFenceValue_, Fence& fencec)
{
    while (true)
    {
        if (!MessageLoop()) break;

        // 現在のバックバッファインデックスを取得
        const auto backBufferIndex = directX.sc->GetCurrentBackBufferIndex();

        // 以前のフレームの GPU の処理が完了しているか確認して待機する
        if (frameFenceValue_[backBufferIndex] != 0) {
            fencec.wait(frameFenceValue_[backBufferIndex]);
        }

        // コマンドアロケータリセット
        commandAllocatorInstance_[backBufferIndex].reset();
        // コマンドリストリセット
        commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

        // リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
        auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        commandListInstance_.get()->ResourceBarrier(1, &pToRT);

        // レンダーターゲットの設定
        D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(deviceInstance_, descriptorHeapInstance_, backBufferIndex) };
        commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

        // レンダーターゲットのクリア
        const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // クリア
        commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

        //-------------------------------------------------

        // パイプラインステートの設定
        commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());
        // ルートシグネチャの設定
        commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

        // ビューポートの設定
        const auto [w, h] = windowInstance_.size();
        D3D12_VIEWPORT viewport{};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(w);
        viewport.Height = static_cast<float>(h);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        commandListInstance_.get()->RSSetViewports(1, &viewport);

        // シザー矩形の設定
        D3D12_RECT scissorRect{};
        scissorRect.left = 0;
        scissorRect.top = 0;
        scissorRect.right = w;
        scissorRect.bottom = h;
        commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

        // ポリゴンの描画
        trianglePolygonInstance_.draw(commandListInstance_);

        //-------------------------------------------------

        // リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
        auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        commandListInstance_.get()->ResourceBarrier(1, &rtToP);

        // コマンドリストをクローズ
        commandListInstance_.get()->Close();

        // コマンドキューにコマンドリストを送信
        ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
        commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // プレゼント
        swapChainInstance_.get()->Present(1, 0);

        // フェンスにフェンス値を設定
        commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
        frameFenceValue_[backBufferIndex] = nextFenceValue_;
        nextFenceValue_++;
    }

    // ループを抜けるとウィンドウを閉じる
}


int WINAPI WinMain(
    _In_ HINSTANCE hInstance,      // アプリケーションの識別番号
    _In_opt_ HINSTANCE hPrevInstance,  // 基本使わなくていい
    _In_ LPSTR lpCmdLine,          // コマンドライン引数（起動時のオプション）
    _In_ int nCmdShow              // ウィンドウの表示方法（最大化、最小化など）
)
{
    // ここにメインの処理を書く

    // 1. ウィンドウクラス登録
    RegisterWindowClass(hInstance);

    // 2. ウィンドウ作成
    HWND hwnd = FCreateWindow(hInstance, nCmdShow);

    // DirectX.
    DirectX dxc;
    dxc.dx(hwnd);

    // 画面クリア.
    DisplayClear clearc;

    DescriptorHeap dhc;
    dhc.create(*dxc.d3d12d);

    CommandAllocator cac;
    cac.test(*dxc.d3d12d);

    RenderTarget rtc;
    rtc.create(*dxc.sc, *dxc.d3d12d, dhc.rtvHeap);

    CommandList clc;
    clc.test();

    shader shaderc;
    shaderc.cretate(*dxc.d3d12d);

    rootSignature rootSignaturec;
    rootSignaturec.create(*dxc.d3d12d);

    pipelineStateObject pipelineStateObjectc;
    pipelineStateObjectc.create(*dxc.d3d12d, *shaderc.vertexShader_, *shaderc.pixelShader_, *rootSignaturec.rootSignature);

    trianglePolygon trianglePolygonc;
    trianglePolygonc.create(*dxc.d3d12d);

    Fence fencec;
    fencec.create(*dxc.d3d12d);

    UINT64 frameFenceValue[2];
    UINT64 nextFenceValue;

    // ゲームループ開始.
    MainGameLoop(dxc, frameFenceValue, nextFenceValue, fencec);



    return 0;
}