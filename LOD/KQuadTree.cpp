#include "KQuadTree.h"

bool KQuadTree::UpdateIndexList(KNode* pNode)
{
	//
	int iNumCols = m_Map->m_Mapinfo.m_NumCol;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	m_IndexList.resize(iNumColCell * iNumRowCell * 2 * 3);

	int iIndex = 0;
	for (int iRow = iStartRow; iRow < iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol < iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * (iNumColCell + 1) + iCol;
			int iNextRow = (iRow + 1) * (iNumColCell + 1) + iCol;
			m_IndexList[iIndex + 0] = iCurrentIndex;
			m_IndexList[iIndex + 1] = iCurrentIndex + 1;
			m_IndexList[iIndex + 2] = iNextRow;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow + 1;
			iIndex += 6;
		}
	}
	if (m_IndexList.size() > 0) return true;
	return false;
}
HRESULT KQuadTree::CreateIndexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_IndexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}

bool KQuadTree::UpdateVertexList(KNode* pNode)
{
	int numCols = m_Map->m_Mapinfo.m_NumCol;
	int startRow = pNode->m_CornerList[0] / numCols;
	int endRow = pNode->m_CornerList[2] / numCols;
	int startCol = pNode->m_CornerList[0] % numCols;
	int endCol = pNode->m_CornerList[1] % numCols;

	int numColCell = endCol - startCol;
	int numRowCell = endRow - startRow;
	pNode->m_pVertexList.resize((endCol - startCol + 1) * (endRow - startRow + 1));

	int index = 0;
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			int currentIndex = row * numCols + col;
			pNode->m_pVertexList[index++] = m_Map->m_VertexList[currentIndex];
		}
	}
	if (pNode->m_pVertexList.size() > 0) return true;
	return false;
}

HRESULT KQuadTree::CreateVertexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PS_VECTOR) * pNode->m_pVertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &pNode->m_pVertexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &pNode->m_VertexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}

void KQuadTree::SetNeighborNode(KNode* pNode)
{
	for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
	{
		KNode* pNode = m_LeafList[iNode];
		TVector3 vLT = m_Map->m_VertexList[pNode->m_CornerList[0]].pos;
		TVector3 vRT = m_Map->m_VertexList[pNode->m_CornerList[1]].pos;
		TVector3 vlB = m_Map->m_VertexList[pNode->m_CornerList[2]].pos;
		TVector3 vRB = m_Map->m_VertexList[pNode->m_CornerList[3]].pos;
		TVector3 vCenter = (vLT + vRT + vlB + vRB);
		vCenter /= 4.0f;
		// RIGHT
		TVector2 vPoint;
		vPoint.x = vCenter.x + (vRT.x - vLT.x);
		vPoint.y = vCenter.z;
		for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
		{
			if (m_LeafList[iNode] == pNode) continue;
			if (m_LeafList[iNode]->IsRect(vPoint))
			{
				pNode->m_NeighborList[0] = m_LeafList[iNode];
				break;
			}
		}
		// LEFT
		vPoint.x = vCenter.x - (vRT.x - vLT.x);
		vPoint.y = vCenter.z;
		for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
		{
			if (m_LeafList[iNode] == pNode) continue;
			if (m_LeafList[iNode]->IsRect(vPoint))
			{
				pNode->m_NeighborList[1] = m_LeafList[iNode];
				break;
			}
		}
		// BOTTOM
		vPoint.x = vCenter.x;
		vPoint.y = vCenter.z - (vLT.z - vRB.z);
		for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
		{
			if (m_LeafList[iNode] == pNode) continue;
			if (m_LeafList[iNode]->IsRect(vPoint))
			{
				pNode->m_NeighborList[2] = m_LeafList[iNode];
				break;
			}
		}
		//TOP
		vPoint.x = vCenter.x;
		vPoint.y = vCenter.z + (vLT.z - vRB.z);
		for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
		{
			if (m_LeafList[iNode] == pNode) continue;
			if (m_LeafList[iNode]->IsRect(vPoint))
			{
				pNode->m_NeighborList[3] = m_LeafList[iNode];
				break;
			}
		}
	}
}

bool KQuadTree::ComputeStaticLodIndex(int iMaxCells)
{
	m_LodPatchList.reserve(m_NumPatch);
	return true;
}

bool KQuadTree::LoadObject(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		return false;
	}

	TCHAR data[256] = { 0, };

	TCHAR buffer[256] = { 0, };
	int iVersion = 0;
	_fgetts(buffer, 256, fp);
	TCHAR tmp[256] = { 0, };
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

	int iNumPatch = 0;
	_fgetts(buffer, 256, fp);
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iNumPatch);

	int index = 0;

	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		LodPatch lod;
		lod.lodLevel = iLod;
		for (int iCode = 0; iCode < 16; iCode++)
		{
			std::vector<std::wstring>	ListTokens;
			_fgetts(buffer, 256, fp);
			_stscanf_s(buffer, _T("%d %s"), &index, data, _countof(data));

			std::wstring sentence = data;
			Tokenize(sentence, L",", std::back_inserter(ListTokens));
			int iMaxCnt = (int)ListTokens.size();
			lod.IndexList[iCode].resize(iMaxCnt);
			for (int i = 0; i < iMaxCnt; i++)
			{
				lod.IndexList[iCode][i] = (DWORD)(_tstoi(ListTokens[i].c_str()));
			}
		}
		m_LodPatchList.push_back(lod);

		_fgetts(buffer, 256, fp);
	}
	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		for (int iCode = 0; iCode < 16; iCode++)
		{
			CreateIndexBuffer(m_LodPatchList[iLod], iCode);
		}
	}
	fclose(fp);
	return true;
}
HRESULT KQuadTree::CreateIndexBuffer(LodPatch& patch, int iCode)
{

	patch.IndexBufferList[iCode] = nullptr;
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * patch.IndexList[iCode].size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &patch.IndexList[iCode].at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &patch.IndexBufferList[iCode]);
	if (FAILED(hr)) return hr;
	return hr;
}
bool	KQuadTree::Render(ID3D11DeviceContext* pContext, DebugCamera* m_pCamera)
{
	for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
	{
		int iLodLevel = 0;
		float fDistance = (m_LeafList[iNode]->m_Center - m_pCamera->m_CameraPos).Length();
		if (fDistance < 30.0f)
		{
			m_LeafList[iNode]->m_LodLevel = 2;
		}
		else if (fDistance < 60.0f)
		{
			m_LeafList[iNode]->m_LodLevel = 1;
		}
		else
			m_LeafList[iNode]->m_LodLevel = 0;
	}
	for (int iNode = 0; iNode < m_LeafList.size(); iNode++)
	{
		int iRenderCode = 0;
		// 동서남북
		if (m_LeafList[iNode]->m_NeighborList[0] &&
			m_LeafList[iNode]->m_LodLevel < m_LeafList[iNode]->m_NeighborList[0]->m_LodLevel)
		{
			iRenderCode += 2;
		}
		if (m_LeafList[iNode]->m_NeighborList[1] &&
			m_LeafList[iNode]->m_LodLevel < m_LeafList[iNode]->m_NeighborList[1]->m_LodLevel)
		{
			iRenderCode += 8;
		}
		if (m_LeafList[iNode]->m_NeighborList[2] &&
			m_LeafList[iNode]->m_LodLevel < m_LeafList[iNode]->m_NeighborList[2]->m_LodLevel)
		{
			iRenderCode += 4;
		}
		if (m_LeafList[iNode]->m_NeighborList[3] &&
			m_LeafList[iNode]->m_LodLevel < m_LeafList[iNode]->m_NeighborList[3]->m_LodLevel)
		{
			iRenderCode += 1;
		}

		UINT iNumIndex = 0;
		ID3D11Buffer* pRenderBuffer = nullptr;
		UINT iLodLevel = m_LeafList[iNode]->m_LodLevel;
		if (m_LeafList[iNode]->m_LodLevel == 0)
		{
			iNumIndex = m_LodPatchList[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].IndexBufferList[iRenderCode];
		}
		else if (m_LeafList[iNode]->m_LodLevel == 1)
		{
			iNumIndex = m_LodPatchList[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].IndexBufferList[iRenderCode];
		}
		else
		{
			iNumIndex = m_IndexList.size();
			pRenderBuffer = m_IndexBuffer;
		}

		m_Map->preRander(pContext);
		UINT pStrides = sizeof(PS_VECTOR);
		UINT pOffsets = 0;
		pContext->IASetVertexBuffers(0, 1, &m_LeafList[iNode]->m_VertexBuffer,
			&pStrides, &pOffsets);
		pContext->IASetIndexBuffer(pRenderBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_Map->postRander(pContext, iNumIndex);
	}
	return true;
}
bool	KQuadTree::Frame()
{
	return true;
}
void    KQuadTree::Build(KMap* pMap)
{
	LoadObject(L"Lod.txt");
	m_Map = pMap;
	m_NumCol = pMap->m_Mapinfo.m_NumCol;
	m_NumRow = pMap->m_Mapinfo.m_NumRow;
	m_RootNode = CreateNode(nullptr, 0, m_NumCol - 1, (m_NumRow - 1) * m_NumCol, m_NumRow * m_NumCol - 1);
	Buildtree(m_RootNode);
	SetNeighborNode(m_RootNode);
	m_NumCell = (m_NumCol - 1) / pow(2.0f, m_MaxDepth);
	m_NumPatch = (log(m_NumCell) / log(2.0f));
	if (m_NumPatch > 0)
	{
		m_LodPatchList.resize(m_NumPatch);
	}
	//공유 인덱스 버퍼
	if (UpdateIndexList(m_LeafList[0]))
	{
		CreateIndexBuffer(m_LeafList[0]);
	}
}
bool    KQuadTree::Init()
{
	return true;
}
bool  KQuadTree::SubDivide(KNode* pNode)
{
	if ((pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 4)
	{
		return true;
	}
	return false;
}
void KQuadTree::Buildtree(KNode* pNode)
{
	if (SubDivide(pNode))
	{

		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_Child[0] = CreateNode(pNode, pNode->m_CornerList[0], mt, ml, center);
		Buildtree(pNode->m_Child[0]);

		pNode->m_Child[1] = CreateNode(pNode, mt, pNode->m_CornerList[1], center, mr);
		Buildtree(pNode->m_Child[1]);

		pNode->m_Child[2] = CreateNode(pNode, ml, center, pNode->m_CornerList[2], mb);
		Buildtree(pNode->m_Child[2]);

		pNode->m_Child[3] = CreateNode(pNode, center, mr, mb, pNode->m_CornerList[3]);
		Buildtree(pNode->m_Child[3]);
	}
	else
	{
		pNode->m_Leaf = true;
		m_MaxDepth = pNode->m_Depth;
		TVector3 vLT = m_Map->m_VertexList[pNode->m_CornerList[0]].pos;
		TVector3 vRT = m_Map->m_VertexList[pNode->m_CornerList[1]].pos;
		TVector3 vLB = m_Map->m_VertexList[pNode->m_CornerList[2]].pos;
		TVector3 vRB = m_Map->m_VertexList[pNode->m_CornerList[3]].pos;

		pNode->SetRect(vLT.x, vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);

		// 공유 인덱스버퍼용(정점버퍼 리프노드 당)
		if (UpdateVertexList(pNode))
		{
			CreateVertexBuffer(pNode);
		}
		m_LeafList.push_back(pNode);
	}
}
KNode* KQuadTree::FindNode(KNode* pNode, TVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_Child[iNode] != nullptr &&
				pNode->m_Child[iNode]->IsRect(pos))
			{
				m_Queue.push(pNode->m_Child[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    KQuadTree::AddObject(TVector2 pos)
{
	KNode* pFindNode = FindNode(m_RootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
KNode* KQuadTree::FindPlayerNode(TVector2 pos)
{
	KNode* pFindNode = FindNode(m_RootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
bool KQuadTree::Release()
{
	for (int iPatch = 0; iPatch < m_LodPatchList.size(); iPatch++)
	{
		m_LodPatchList[iPatch];
	}
	if (m_IndexBuffer)m_IndexBuffer->Release();
	delete m_RootNode;
	m_RootNode = nullptr;
	return true;
}
KNode* KQuadTree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_Depth = pParent->m_Depth + 1;
		pNode->m_Parent = pParent;
	}
	pNode->m_Index = KNode::g_NewCounter;
	KNode::g_NewCounter++;
	return pNode;
}
KQuadTree::KQuadTree()
{
	m_RootNode = nullptr;
}

KQuadTree::~KQuadTree()
{
}
