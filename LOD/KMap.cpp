#include "KMap.h"
bool KMap::Load(KMapInfo& info)
{
    m_Mapinfo = info;
    m_Mapinfo.m_NumColCell = m_Mapinfo.m_NumCol - 1;
    m_Mapinfo.m_NumRowCell = m_Mapinfo.m_NumRow - 1;
    m_Mapinfo.m_NumVertex = m_Mapinfo.m_NumColCell * m_Mapinfo.m_NumRowCell * 6;

    return true;
}
bool KMap::CreateIndexData()
{
    m_IndexList.resize(m_Mapinfo.m_NumColCell * m_Mapinfo.m_NumRowCell * 2 * 3);
    int iIndex = 0;
    for (int iRow = 0; iRow < m_Mapinfo.m_NumRowCell; iRow++)
    {
        for (int iCol = 0; iCol < m_Mapinfo.m_NumColCell; iCol++)
        {
            int iCurrentIndex = iRow * m_Mapinfo.m_NumRow + iCol;
            int iNextRow = (iRow + 1) * m_Mapinfo.m_NumRow + iCol;
            m_IndexList[iIndex + 0] = iCurrentIndex;
            m_IndexList[iIndex + 1] = iCurrentIndex + 1;
            m_IndexList[iIndex + 2] = iNextRow;
            m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
            m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
            m_IndexList[iIndex + 5] = iNextRow + 1;
            iIndex += 6;
        }
    }
    if (m_IndexList.size() > 0) { return true; }

    return false;

}
bool KMap::CreateVertexData()
{
    m_VertexList.resize(m_Mapinfo.m_NumVertex);

    int iIndex = 0;
    for (int iRow = 0; iRow < m_Mapinfo.m_NumRow; iRow++)
    {
        for (int iCol = 0; iCol < m_Mapinfo.m_NumCol; iCol++)
        {
            int iIndex = iRow * m_Mapinfo.m_NumRow + iCol;
            m_VertexList[iIndex].pos.x = m_Mapinfo.m_CellDistance * iCol;
            m_VertexList[iIndex].pos.y = 0.0f;
            m_VertexList[iIndex].pos.z = -m_Mapinfo.m_CellDistance * iRow;
            m_VertexList[iIndex].Color = TVector4(randstep(0.0f, 1.0f), randstep(0.0f, 1.0), randstep(0.0f, 1.0f), 1.0f);
        }
    }
    if (m_VertexList.size() > 0) { return true; }

    return false;
}