#include "TilemapPCH.h"
#include "SharedTilemap/TilemapAgentTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::tilemap::AgentTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const shared::tilemap::AgentTemplate& value)
{
}