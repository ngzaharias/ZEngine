#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenCountComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(client::hidden::CountComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const client::hidden::CountComponent& value)
{
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::CountComponent& value)
{
	return false;
}