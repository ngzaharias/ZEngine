#include "TilemapPCH.h"
#include "Tilemap/TilemapAgentComponent.h"

#include "Engine/Visitor.h"

template<>
void eng::Visitor::ReadCustom(tilemap::AgentComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const tilemap::AgentComponent& value)
{
}