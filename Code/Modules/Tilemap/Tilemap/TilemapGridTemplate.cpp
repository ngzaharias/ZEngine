#include "TilemapPCH.h"
#include "Tilemap/TilemapGridTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(tilemap::GridTemplate& value) const
{
	Read("m_GridSize", value.m_GridSize, value.m_GridSize);
	Read("m_TileSize", value.m_TileSize, value.m_TileSize);
}
template<>
void Visitor::WriteCustom(const tilemap::GridTemplate& value)
{
	Write("m_GridSize", value.m_GridSize);
	Write("m_TileSize", value.m_TileSize);
}