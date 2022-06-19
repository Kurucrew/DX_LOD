#pragma once
#include "KCamera.h"
#include "KNode.h"
class KQuadTree
{
public:
	std::vector<LodPatch>   m_LodPatchList;
	UINT m_MaxDepth;
	UINT m_NumCell;
	UINT m_NumPatch;
	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_IndexBuffer;
	KNode* m_RootNode;
	int	m_NumCol;
	int	m_NumRow;
	std::queue<KNode*>  m_Queue;
	std::vector<KNode*>  m_LeafList;
	KMap* m_Map;

	bool	UpdateIndexList(KNode* pNode);
	HRESULT CreateIndexBuffer(KNode* pNode);
	HRESULT CreateIndexBuffer(LodPatch& patch, int iCode);
	bool	UpdateVertexList(KNode* pNode);
	HRESULT CreateVertexBuffer(KNode* pNode);
	void	SetNeighborNode(KNode* pNode);
	bool    ComputeStaticLodIndex(int iMaxCells);
	bool    LoadObject(std::wstring filename);
	template <typename OutputIterator>
	void	Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
	void    Build(KMap* pMap);
	bool    Init();
	KNode* CreateNode(KNode* pParent, float x, float y, float w, float h);
	void	Buildtree(KNode*);
	bool    AddObject(TVector2 pos);
	KNode* FindNode(KNode* pNode, TVector2 pos);
	KNode* FindPlayerNode(TVector2 pos);
	bool	SubDivide(KNode* pNode);
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext, DebugCamera* m_pCamera);
	bool    Release();
	KQuadTree();
	virtual ~KQuadTree();
};
template<typename OutputIterator>
inline void KQuadTree::Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first)
{
	size_t start, stop, n = text.length();

	for (start = text.find_first_not_of(delimiters); 0 <= start && start < n;
		start = text.find_first_not_of(delimiters, stop + 1))
	{
		stop = text.find_first_of(delimiters, start);
		if (stop < 0 || stop > n) { stop = n; }
		*first++ = text.substr(start, stop - start);
	}
}
