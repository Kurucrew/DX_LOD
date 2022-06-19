#include "KCamera.h"
#include "KInput.h"
bool KCamera::Init()
{
    return true;
}
TMatrix KCamera::CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
    m_CameraPos = vPos;
    m_CameraTarget = vTarget;
    D3DXMatrixLookAtLH(&m_matView, &m_CameraPos, &m_CameraTarget, &vUp);

    m_Side.x = m_matView._11;
    m_Side.y = m_matView._21;
    m_Side.z = m_matView._31;

    m_Up.x = m_matView._12;
    m_Up.y = m_matView._22;
    m_Up.z = m_matView._32;

    m_Look.x = m_matView._13;
    m_Look.y = m_matView._23;
    m_Look.z = m_matView._33;
    return m_matView;

    return m_matView;
}
TMatrix KCamera::CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect)
{
    D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);

    return m_matProj;
}
TMatrix DebugCamera::Update(TVector4 vValue)
{
    TQuaternion q;
    D3DXQuaternionRotationYawPitchRoll(&q, vValue.y, vValue.x, vValue.z);
    TMatrix matRotation;
    D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_CameraPos);
    D3DXMatrixInverse(&m_matView, NULL, &matRotation);
    
    m_Side.x = m_matView._11;
    m_Side.y = m_matView._21;
    m_Side.z = m_matView._31;

    m_Up.x = m_matView._12;
    m_Up.y = m_matView._22;
    m_Up.z = m_matView._32;

    m_Look.x = m_matView._13;
    m_Look.y = m_matView._23;
    m_Look.z = m_matView._33;

    return matRotation;
}
bool KCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_CameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_CameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_CameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_CameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_CameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_CameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_CameraPos.y -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('R') >= KEY_HOLD)
    {
        m_CameraPos.y += m_pSpeed * g_fSecPerFrame;
    }

    m_matView = CreateViewMatrix(m_CameraPos, m_CameraTarget);

    m_Side.x = m_matView._11;
    m_Side.y = m_matView._21;
    m_Side.z = m_matView._31;

    m_Up.x = m_matView._12;
    m_Up.y = m_matView._22;
    m_Up.z = m_matView._32;

    m_Look.x = m_matView._13;
    m_Look.y = m_matView._23;
    m_Look.z = m_matView._33;

    return true;
}
bool KCamera::Render()
{
    return true;
}
bool KCamera::Release()
{
    return true;
}
KCamera::KCamera()
{
    m_pSpeed = 30.0f;
    m_CameraPos = { 0, 20, -20.0f };
    m_CameraTarget = { 0, 30, 1.0f };
}
KCamera::~KCamera()
{
}
bool DebugCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_CameraPos = m_CameraPos + m_Look * m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_CameraPos = m_CameraPos + m_Look * -m_pSpeed * g_fSecPerFrame;
    }

    TVector3 vLook;
    vLook.x = m_Look.x;
    vLook.y = m_Look.y;
    vLook.z = m_Look.z;
    TVector3 vTarget;
    vTarget.x = m_CameraPos.x;
    vTarget.y = m_CameraPos.y;
    vTarget.z = m_CameraPos.z;

    TMatrix matRotation, matY, matX;
    if (g_Input.GetKey(VK_RIGHT) >= KEY_PUSH)
    {
        matRotation = TMatrix::CreateRotationY(g_fSecPerFrame);
    }
    if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
    {
        matRotation = TMatrix::CreateRotationY(-g_fSecPerFrame);
    }

    return true;
}