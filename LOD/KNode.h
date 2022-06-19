#pragma once
#include "Std.h"
#include "KMap.h"
struct SRect
{
	float  x, y, w, h;
};
struct VRect
{
	TVector2  p0;
	TVector2  p1;
	TVector2  wh;
};
typedef std::vector<DWORD> DWORD_VECTOR;
struct LodPatch
{
	UINT lodLevel;
	DWORD_VECTOR IndexList[16];
	ID3D11Buffer* IndexBufferList[16];
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (IndexBufferList[iBuffer])
			{
				IndexBufferList[iBuffer]->Release();
				IndexBufferList[iBuffer] = nullptr;
			}

		}
	}
	LodPatch() {}
	~LodPatch()
	{
	}
};
class KNode
{
public:
	UINT m_LodLevel;
	static int g_NewCounter;
	int	m_Index;
	VRect m_Rect;
	std::vector<TVector2>  m_ObjectList;
	std::vector<DWORD>  m_CornerList; 
	std::vector<DWORD>  m_IndexList; 
	ID3D11Buffer* m_IndexBuffer;
	std::vector<PS_VECTOR>  m_pVertexList;
	ID3D11Buffer* m_VertexBuffer;
	TVector3 m_Center;
	KNode* m_Child[4];
	KNode* m_NeighborList[4];
	int	m_Depth;
	bool m_Leaf;
	KNode* m_Parent;

	bool AddObject(float fx, float fy);
	bool AddObject(TVector2 pos);
	bool IsRect(TVector2 pos);
	void SetRect(float x, float y, float w, float h);
	KNode() {};
	KNode(UINT x, UINT y, UINT w, UINT h);
	~KNode();
};

