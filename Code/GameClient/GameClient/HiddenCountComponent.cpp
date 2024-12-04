#include "GameClientPCH.h"
#include "GameClient/HiddenCountComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(hidden::CountComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const hidden::CountComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(hidden::CountComponent& value)
{
	return false;
}