#include "KWindow.h"
#include <assert.h>
KWindow* g_pWindow = nullptr;
HWND  g_hWnd;
RECT  g_rtClient;
LRESULT CALLBACK staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    assert(g_pWindow);
    return g_pWindow->WndProc(hWnd, message, wParam, lParam);
}
LRESULT KWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MsgProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT KWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
bool KWindow::InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int ix, int iy)
{
    m_hInstance = hInstance;

    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = staticWndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = CreateSolidBrush(RGB(133, 143, 108));
    wcex.lpszClassName = L"KGCA";
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassExW(&wcex)) { return false; }

    RECT rc = { 0, 0, ix, iy };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindowEx(
        0,
        L"KGCA",
        strWindowTitle,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (m_hWnd == NULL) { return false; }

    GetWindowRect(m_hWnd, &m_rtWindow);
    GetClientRect(m_hWnd, &m_rtClient);

    g_hWnd = m_hWnd;
    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, nCmdShow);

    return true;
}
bool KWindow::Run()
{
    GameInit();
    MSG msg;
    while (m_bGameRun)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else { m_bGameRun = GameRun(); }
    }
    GameRelease();

    return true;
}
bool KWindow::GameRun()
{
    return true;
}
bool KWindow::GameInit()
{
    return true;
}
bool KWindow::GameRelease()
{
    return true;
}
KWindow::KWindow() : m_bGameRun(true)
{
    g_pWindow = this;
}