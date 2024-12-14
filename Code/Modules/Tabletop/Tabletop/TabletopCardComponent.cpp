#include "TabletopPCH.h"
#include "Tabletop/TabletopCardComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(tabletop::CardComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const tabletop::CardComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(tabletop::CardComponent& value)
{
	bool result = false;
	return result;
}