#include "KInput.h"
void KInput::OnMove(int ix, int iy)
{
    if (m_bDrag)
    {
        m_pDrag.x = ix - m_pDragDown.x;
        m_pDrag.y = iy - m_pDragDown.y;
    }
    m_pDragDown.x = ix;
    m_pDragDown.y = iy;

    return;
}
void KInput::OnBegin(int ix, int iy)
{
    m_bDrag = true;
    m_pDragDown.x = ix;
    m_pDragDown.y = iy;

    return;
}
void KInput::OnEnd()
{
    m_bMove = false;
    m_bDrag = false;
    m_pDrag.x = 0;
    m_pDrag.y = 0;

    return;
}
LRESULT KInput::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int iMouseX = (short)LOWORD(lParam);
    int iMouseY = (short)HIWORD(lParam);
    switch (message)
    {
    case  WM_MOUSEMOVE:
        OnMove(iMouseX, iMouseY);

        return true;
    case WM_LBUTTONDOWN:
        SetCapture(g_hWnd);
        OnBegin(iMouseX, iMouseY);

        return true;
    case WM_LBUTTONUP:
        ReleaseCapture();
        OnEnd();

        return true;
    case WM_MOUSEWHEEL:
    {
        m_iWheel = 0;
        m_iWheel = GET_WHEEL_DELTA_WPARAM(wParam);

        return 0;
    }
    }

    return 0;
}
bool KInput::Init()
{
    ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);

    return true;
}
DWORD KInput::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
bool KInput::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    for (int iKey = 0; iKey < 256; iKey++)
    {
        SHORT sKey = GetAsyncKeyState(iKey);
        if (sKey & 0x8000)
        {
            if (m_dwKeyState[iKey] == KEY_FREE)
            {
                m_dwKeyState[iKey] = KEY_PUSH;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_HOLD;
            }
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH ||
                m_dwKeyState[iKey] == KEY_HOLD)
            {
                m_dwKeyState[iKey] = KEY_UP;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_FREE;
            }
        }
    }

    return true;
}
bool KInput::Render()
{
    return true;
}
bool KInput::Release()
{
    return true;
}
KInput::KInput() {}
KInput::~KInput() {}