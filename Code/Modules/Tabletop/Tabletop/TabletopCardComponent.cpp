#include "TabletopPCH.h"
#include "Tabletop/TabletopCardComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(tabletop::CardComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const tabletop::CardComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(tabletop::CardComponent& value)
{
	bool result = false;
	return result;
}