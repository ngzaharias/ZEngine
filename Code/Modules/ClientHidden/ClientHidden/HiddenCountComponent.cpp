#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenCountComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(client::hidden::CountComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const client::hidden::CountComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::CountComponent& value)
{
	return false;
}