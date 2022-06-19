#pragma once
#include "Std.h"
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class KInput : public Singleton<KInput>
{
	friend class Singleton<KInput>;

	DWORD m_dwKeyState[256];
public:
	POINT m_ptBeforePos;
	POINT m_ptPos;
	POINT m_pDragDown;
	POINT m_pDrag;
	bool m_bDrag;
	bool m_bMove;
	int m_iWheel;

	DWORD   GetKey(DWORD dwKey);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void OnMove(int ix, int iy);
	void OnBegin(int ix, int iy);
	void OnEnd();
private:
	KInput();
public:
	~KInput();
};
#define g_Input KInput::Get()

