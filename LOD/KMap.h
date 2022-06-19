#pragma once
#include "Model.h"
struct KMapInfo
{
	int m_NumRow;
	int m_NumCol;
	int m_NumRowCell;
	int m_NumColCell;
	int m_NumVertex;
	float m_CellDistance;
};
class KMap : public Model
{
public:
	KMapInfo m_Mapinfo;

	bool CreateIndexData() override;
	bool CreateVertexData() override;
	bool Load(KMapInfo& info);
};

