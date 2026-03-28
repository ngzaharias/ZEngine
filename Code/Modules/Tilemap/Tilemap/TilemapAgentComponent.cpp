#include "TilemapPCH.h"
#include "Tilemap/TilemapAgentComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(tilemap::AgentComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const tilemap::AgentComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(tilemap::AgentComponent& value)
{
	return false;
}