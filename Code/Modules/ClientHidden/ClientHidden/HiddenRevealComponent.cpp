#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenRevealComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(client::hidden::RevealComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const client::hidden::RevealComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::RevealComponent& value)
{
	return false;
}