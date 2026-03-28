#include "TabletopPCH.h"
#include "Tabletop/TabletopDeckComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(tabletop::DeckComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const tabletop::DeckComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(tabletop::DeckComponent& value)
{
	bool result = false;
	return result;
}