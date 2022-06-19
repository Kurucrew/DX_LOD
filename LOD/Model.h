#pragma once
#include "Std.h"
struct PS_VECTOR
{
	TVector3 pos;
	TVector4 Color;
	PS_VECTOR(TVector3 p, TVector4 c)
	{
		pos = p;
		Color = c;
	}
	PS_VECTOR()
	{
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;
		Color.w = 1.0f;
	}
};
struct CB_DATA
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;
	TVector4 VValue;
};
class Model
{
public:
UINT m_NumIndex;
ID3DBlob* m_VSBlob = nullptr;
ID3D11Buffer* m_VertexBuffer;
ID3D11Buffer* m_IndexBuffer;
ID3D11Buffer* m_ConstantBuffer;
ID3D11InputLayout* m_VertexLayout;
ID3D11VertexShader* m_Vs;
ID3D11PixelShader* m_Ps;
CB_DATA m_CbData;
std::vector<PS_VECTOR> m_VertexList;
std::vector<DWORD> m_IndexList;

TMatrix m_matWorld;
TMatrix m_matView;
TMatrix m_matProj;

void SetMatrix(TMatrix* MatWorld, TMatrix* MatView, TMatrix* MatProj);
virtual bool CreateVertexData();
virtual bool CreateIndexData();
bool  LoadObject(std::wstring filename);
HRESULT CreateConstantBuffer();
HRESULT CreateVertexBuffer();
HRESULT CreateIndexBuffer();
HRESULT CreateVertexLayout();
HRESULT LoadShader();
bool Init();
bool Frame();
bool preRander(ID3D11DeviceContext* Context);
bool Render(ID3D11DeviceContext* Context);
bool postRander(ID3D11DeviceContext* Context, UINT NumValue);
bool Release();
};

