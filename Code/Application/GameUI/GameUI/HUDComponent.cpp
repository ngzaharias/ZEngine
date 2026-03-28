#include "GameUIPCH.h"
#include "GameUI/HUDComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(gui::HUDComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const gui::HUDComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(gui::HUDComponent& value)
{
	return false;
}