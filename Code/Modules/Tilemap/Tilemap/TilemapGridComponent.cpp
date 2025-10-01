#include "TilemapPCH.h"
#include "Tilemap/TilemapGridComponent.h"

#include "Engine/Visitor.h"

template<>
void eng::Visitor::ReadCustom(tilemap::GridComponent& value) const
{
	Read("m_GridSize", value.m_GridSize, value.m_GridSize);
	Read("m_TileSize", value.m_TileSize, value.m_TileSize);
}
template<>
void eng::Visitor::WriteCustom(const tilemap::GridComponent& value)
{
	Write("m_GridSize", value.m_GridSize);
	Write("m_TileSize", value.m_TileSize);
}