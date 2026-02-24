#include "GameUIPCH.h"
#include "GameUI/HUDComponent.h"

#include "Engine/Visitor.h"
#include "ImGui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(gui::HUDComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const gui::HUDComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(gui::HUDComponent& value)
{
	return false;
}