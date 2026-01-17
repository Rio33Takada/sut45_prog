#include <Windows.h>
#include "DX.h"
#include "DisplayClear.h"
#include "pipeline_state_object.h"
#include "root_signature.h"
#include "trianglePolygon.h"
#include "shader.h"

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

void MainGameLoop() 
{
    while (true)
    {
        if (!MessageLoop()) break;
    }
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

    shader shaderc;
    shaderc.cretate(* dxc.d3d12d);

    rootSignature rootSignaturec;
    rootSignaturec.create(*dxc.d3d12d);

    pipelineStateObject pipelineStateObjectc;
    pipelineStateObjectc.create(* dxc.d3d12d, * shaderc.vertexShader_, * shaderc.pixelShader_, *rootSignaturec.rootSignature);

    trianglePolygon trianglePolygonc;
    trianglePolygonc.create(* dxc.d3d12d);

    // ゲームループ開始.
    MainGameLoop();



    return 0;
}