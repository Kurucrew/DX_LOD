#include "Model.h"
void Model::SetMatrix(TMatrix* MatWorld, TMatrix* MatView, TMatrix* Proj)
{
    if (MatWorld != nullptr)
    {
        m_CbData.matWorld = MatWorld->Transpose();
    }
    if (MatView != nullptr)
    {
        m_CbData.matView = MatView->Transpose();
    }
    if (Proj != nullptr)
    {
        m_CbData.matProj = Proj->Transpose();
    }
}
bool  Model::LoadObject(std::wstring filename)
{
    FILE* fp = nullptr;
    _tfopen_s(&fp, filename.c_str(), _T("rt"));
    if (fp == NULL) { return false; }

    TCHAR buffer[256] = { 0, };
    int iVersion = 0;
    _fgetts(buffer, 256, fp);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, fp);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for (int iLine = 0; iLine < iNumVertex; iLine++)
    {
        PS_VECTOR v;
        _fgetts(buffer, 256, fp);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.Color.x, &v.Color.y, &v.Color.z, &v.Color.w);
        m_VertexList.push_back(v);
    }
    fclose(fp);

    return true;
}
HRESULT Model::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_IndexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_IndexBuffer);
    if (FAILED(hr)) { return hr; }

    return hr;
}
HRESULT Model::CreateConstantBuffer() //constantbuffer: 정점,픽셀쉐이더에서 사용될 상수를 모아놓은 버퍼
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_CbData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_ConstantBuffer);
    if (FAILED(hr)) { return hr; }

    return hr;
}
HRESULT Model::CreateVertexBuffer()
{

    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(PS_VECTOR) * m_VertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_VertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    if (FAILED(hr)) { return hr; }

    return hr;
}
HRESULT Model::CreateVertexLayout()
{
    HRESULT hr = S_OK;

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    int Size = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
    hr = g_pd3dDevice->CreateInputLayout(vertexDesc, Size, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &m_VertexLayout);
    if (FAILED(hr)) { return hr; }
    m_VSBlob->Release();

    return hr;
}
HRESULT Model::LoadShader()
{
    HRESULT hr = S_OK;
    ID3DBlob* VSblob;
    ID3DBlob* error = nullptr;

    hr = D3DCompileFromFile(L"3DShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_VSBlob, nullptr);
    if (FAILED(hr)) { return hr; }

    hr = g_pd3dDevice->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), NULL, &m_Vs);
    if (FAILED(hr)) { return hr; }

    ID3DBlob* PSblob;
    hr = D3DCompileFromFile(L"PShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &PSblob, &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
        return hr;
    }
    if (FAILED(hr)) { return hr; }

    hr = g_pd3dDevice->CreatePixelShader(PSblob->GetBufferPointer(), PSblob->GetBufferSize(), NULL, &m_Ps);
    if (FAILED(hr)) { return hr; }

    PSblob->Release();

    return hr;
}
bool Model::CreateVertexData()
{
    if (m_VertexList.size() > 0) { return true; }
  
    return false;
}
bool Model::CreateIndexData()
{
    if (m_IndexList.size() > 0) { return true; }

    return false;
}
bool Model::Init()
{
    if (CreateVertexData() && CreateIndexData())
    {
        CreateConstantBuffer();
        CreateVertexBuffer();
        LoadShader();
        CreateVertexLayout();

        return true;
    }

    return false;
}
bool Model::Frame()
{
    m_CbData.VValue.z = g_fGameTimer;

    return true;
}
bool Model::Render(ID3D11DeviceContext* pContext)
{
    if (preRander(pContext) == false) { return true; }
    if (postRander(pContext, m_NumIndex) == false) { return true; }
}
bool Model::preRander(ID3D11DeviceContext* pContext)
{
    if (m_VertexList.size() <= 0) { return true; }
    pContext->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_CbData, 0, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    pContext->VSSetShader(m_Vs, NULL, 0);
    pContext->PSSetShader(m_Ps, NULL, 0);
    pContext->IASetInputLayout(m_VertexLayout);
    UINT pStrides = sizeof(PS_VECTOR);
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
bool Model::postRander(ID3D11DeviceContext* pContext, UINT iNumValue)
{
    pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pContext->DrawIndexed(iNumValue, 0, 0);

    return true;
}
bool Model::Release()
{
    if (m_VertexBuffer)m_VertexBuffer->Release();
    if (m_IndexBuffer) m_IndexBuffer->Release();
    if (m_VertexLayout) m_VertexLayout->Release();
    if (m_ConstantBuffer) m_ConstantBuffer->Release();
    if (m_Vs) m_Vs->Release();
    if (m_Ps) m_Ps->Release();

    return true;
}