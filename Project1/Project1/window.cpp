#include <Windows.h>
#include "DX.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // �~�{�^���������ꂽ��
        PostQuitMessage(0);  // �u�A�v�����I�����܂��v��Windows�ɒʒm
        return 0;
    case WM_PAINT:    // �E�B���h�E�̍ĕ`�悪�K�v�Ȏ�
        // ��ʂ��X�V���鏈���������ɏ���
        return 0;
    case WM_KEYDOWN:  // �L�[�������ꂽ��
        // �L�[���͂̏����������ɏ���
        return 0;
    }
    // �����ŏ������Ȃ����b�Z�[�W��Windows�ɔC����
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void RegisterWindowClass(HINSTANCE hInstance) 
{
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;           // �E�B���h�E�v���V�[�W�����w��i��q�j
    wc.hInstance = hInstance;              // �A�v���P�[�V�����C���X�^���X
    wc.lpszClassName = "GameWindow";      // �E�B���h�E�N���X��
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // �}�E�X�J�[�\��
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // �w�i�F

    RegisterClass(&wc);  // Windows�ɓo�^
}

HWND FCreateWindow(HINSTANCE hInstance, int nCmdShow) 
{
    HWND hwnd = CreateWindow(
        "GameWindow",        // �E�B���h�E�N���X��
        "My Game",           // �E�B���h�E�^�C�g��
        WS_OVERLAPPEDWINDOW,  // �E�B���h�E�X�^�C��
        CW_USEDEFAULT, CW_USEDEFAULT,  // �ʒu�i�����j
        800, 600,            // �T�C�Y�i���A�����j
        NULL, NULL,          // �e�E�B���h�E�A���j���[
        hInstance,           // �C���X�^���X
        NULL                 // �ǉ��f�[�^
    );

    ShowWindow(hwnd, nCmdShow);  // �E�B���h�E��\��
    return hwnd;
}

bool MessageLoop()
{
    // 3. ���b�Z�[�W���[�v
    MSG msg{};  // ���b�Z�[�W���i�[����\����
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // ���b�Z�[�W������܂őҋ@
    {
        if (msg.message == WM_QUIT) return false; // �I�����b�Z�[�W���󂯎������false��Ԃ�.

        TranslateMessage(&msg);  // �L�[�{�[�h���b�Z�[�W���g���₷���`�ɕϊ�
        DispatchMessage(&msg);   // �K�؂ȃE�B���h�E�v���V�[�W���ɑ��M
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
    HINSTANCE hInstance,      // �A�v���P�[�V�����̎��ʔԍ�
    HINSTANCE hPrevInstance,  // ��{�g��Ȃ��Ă���
    LPSTR lpCmdLine,          // �R�}���h���C�������i�N�����̃I�v�V�����j
    int nCmdShow              // �E�B���h�E�̕\�����@�i�ő剻�A�ŏ����Ȃǁj
)
{
    // �����Ƀ��C���̏���������

    // 1. �E�B���h�E�N���X�o�^
    RegisterWindowClass(hInstance);

    // 2. �E�B���h�E�쐬
    HWND hwnd = FCreateWindow(hInstance, nCmdShow);

    // DirectX
    DirectX dxc;
    dxc.dx(hwnd);

    // �Q�[�����[�v�J�n.
    MainGameLoop();

    return 0;
}