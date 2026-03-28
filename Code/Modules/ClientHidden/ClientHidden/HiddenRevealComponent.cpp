#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenRevealComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(client::hidden::RevealComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const client::hidden::RevealComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::RevealComponent& value)
{
	return false;
}