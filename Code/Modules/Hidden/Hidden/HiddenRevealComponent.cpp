#include "HiddenPCH.h"
#include "Hidden/HiddenRevealComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(hidden::RevealComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const hidden::RevealComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(hidden::RevealComponent& value)
{
	return false;
}