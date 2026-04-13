#include "TilemapPCH.h"
#include "Tilemap/TilemapAgentTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(tilemap::AgentTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const tilemap::AgentTemplate& value)
{
}