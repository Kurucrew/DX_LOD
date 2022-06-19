#pragma once
#include "Object3D.h"
class SkyObj : public Object3D
{
public:
	Texture* m_TexArray[6];
	Texture* m_TexCube;
	ComPtr<ID3D11ShaderResourceView> m_SRVArray[6];

	virtual void SetMatrix(T::TMatrix* matworld, T::TMatrix* matview, T::TMatrix* matproj);
	virtual bool SetVertexData();
	virtual bool SetIndexData();
	virtual bool LoadTexture(const TCHAR* ColorFIleName, const TCHAR* MaskFileName);
	bool Render();
	bool PostRender();
	SkyObj();
	virtual ~SkyObj();
};

