#include "Sample.h"
GAMECORE(SampleMap, 800, 600)
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool Sample::Init()
{
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_RSWireFrame);
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_RSSolid);

    KMapInfo info{128 + 1, 128 + 1, 0, 0, 0, 1.0f};
    if (m_Map.Load(info)){ m_Map.Init(); }
    m_Quadtree.Build(&m_Map);
    m_Camera.Init();
    m_Camera.CreateViewMatrix(TVector3(16, 3, -31), TVector3(16, 3, 100000));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    return true;
}
bool Sample::Frame()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    m_Yaw += g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    m_Pitch += g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.Update(TVector4(m_Pitch, m_Yaw, 0.0f, 0.0f));
    m_Camera.Frame();

    m_Map.Frame();

    g_Input.m_ptBeforePos = g_Input.m_ptPos;

    return true;
}
bool Sample::Render()
{
    if (g_Input.GetKey(VK_F2) >= KEY_PUSH)
    {
        if (wireframe == false)
        {
            m_pImmediateContext->RSSetState(m_RSWireFrame);
            wireframe = true;
        }
        else 
        { 
            m_pImmediateContext->RSSetState(m_RSSolid);
            wireframe = false; 
        }
    }
    m_Map.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

    m_Quadtree.Render(m_pImmediateContext, &m_Camera);

    return true;
}
bool Sample::Release()
{
    m_RSWireFrame->Release();
    m_RSSolid->Release();
    m_Map.Release();
    m_Camera.Release();

    return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{

}