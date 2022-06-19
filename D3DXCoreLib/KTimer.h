#pragma once
#include "Std.h"
class KTimer
{
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_Current;
	LARGE_INTEGER m_Frame;
	int	m_iTmpCounter;
public:
	int	  m_iFPS;
	float m_fSecPerFrame;
	float m_fGameTimer;
	TCHAR m_szTimerString[MAX_PATH] = { 0, };

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	KTimer();
	virtual ~KTimer();
};

