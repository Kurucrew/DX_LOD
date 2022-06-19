#include "KNode.h"
int   KNode::g_NewCounter = 0;
bool KNode::AddObject(float fx, float fy)
{
	m_ObjectList.push_back(TVector2(fx, fy));

	return true;
}
bool KNode::AddObject(TVector2 pos)
{
	m_ObjectList.push_back(pos);

	return true;
}
bool KNode::IsRect(TVector2 pos)
{
	if (this->m_Rect.p0.x <= pos.x && this->m_Rect.p1.x >= pos.x && this->m_Rect.p1.y <= pos.y && this->m_Rect.p0.y >= pos.y)
	{
		return true;
	}

	return false;
}
void KNode::SetRect(float x, float y, float w, float h)
{
	this->m_Rect.wh = TVector2(w, h);
	this->m_Rect.p0 = TVector2(x, y);
	this->m_Rect.p1.x = this->m_Rect.p0.x + w;
	this->m_Rect.p1.y = this->m_Rect.p0.y - h;
	m_Center.x = x + (w / 2.0f);
	m_Center.y = 0.0f;
	m_Center.z = y - (h / 2.0f);
}
KNode::KNode(UINT x, UINT y, UINT w, UINT h)
{
	m_Parent = nullptr;
	m_Depth = 0;
	m_Leaf = false;
	m_CornerList.push_back(x);
	m_CornerList.push_back(y);
	m_CornerList.push_back(w);
	m_CornerList.push_back(h);
}
KNode::~KNode()
{
	for (int iChild = 0; iChild < 4; iChild++)
	{
		if (m_Child[iChild] != nullptr)
		{
			delete m_Child[iChild];
			m_Child[iChild] = nullptr;
			KNode::g_NewCounter--;
		}
	}
	if (m_IndexBuffer) m_IndexBuffer->Release();
	if (m_VertexBuffer) m_VertexBuffer->Release();
}