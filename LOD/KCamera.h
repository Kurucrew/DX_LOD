#pragma once
#include "Std.h"
class KCamera
{
public:
	float m_pSpeed;
	TVector3 m_CameraPos;
	TVector3 m_CameraTarget;
	TVector3 m_Look;
	TVector3 m_Side;
	TVector3 m_Up;
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;

	virtual TMatrix CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp = TVector3(0, 1, 0));
	virtual TMatrix CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	KCamera();
	virtual ~KCamera();
};
class DebugCamera : public KCamera
{
public:
	virtual bool Frame() override;
	virtual TMatrix Update(TVector4 vValue);
};
