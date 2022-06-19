#pragma once
#include<windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include<queue>
#include <iterator>
#include <d3dcompiler.h>
#include "KMatrix.h"
#include "KMath.h"
#pragma comment	(lib, "D3DCompiler.lib")
#pragma comment	(lib, "d3d11.lib")
#pragma comment	(lib, "dxgi.lib")
#pragma comment	(lib, "DirectXTK.lib")
#pragma comment	(lib, "D3DXCoreLib.lib")

#define GAME_START int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
#define GAME_RUN(WindowName,x,y)  { Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#WindowName, x, y);g_Sample.Run(); }
#define GAMECORE(WindowName,x,y) GAME_START GAME_RUN(WindowName,x,y)

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

extern float g_fSecPerFrame;
extern float g_fGameTimer;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;

template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};