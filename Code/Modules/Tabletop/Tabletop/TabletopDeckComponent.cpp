#include "TabletopPCH.h"
#include "Tabletop/TabletopDeckComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(tabletop::DeckComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const tabletop::DeckComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(tabletop::DeckComponent& value)
{
	bool result = false;
	return result;
}