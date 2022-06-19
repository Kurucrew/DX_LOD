#pragma once
#include "KCore.h"
#include "KCamera.h"
#include "KMap.h"
#include "KQuadTree.h"
#include "KShape.h"

class Sample : public KCore
{
public:
	ID3D11RasterizerState* m_RSSolid;
	ID3D11RasterizerState* m_RSWireFrame;
	KBoxShape m_BoxObj[2];
	KMap	m_Map;
	DebugCamera m_Camera;
	KQuadTree m_Quadtree;
	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
	bool wireframe = false;

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Sample();
	virtual ~Sample();
};
