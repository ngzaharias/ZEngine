#include "TilemapPCH.h"
#include "SharedTilemap/TilemapGridTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::tilemap::GridTemplate& value) const
{
	Read("m_GridSize", value.m_GridSize, value.m_GridSize);
	Read("m_TileSize", value.m_TileSize, value.m_TileSize);
}
template<>
void Visitor::WriteCustom(const shared::tilemap::GridTemplate& value)
{
	Write("m_GridSize", value.m_GridSize);
	Write("m_TileSize", value.m_TileSize);
}