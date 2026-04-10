#include "GameUIPCH.h"
#include "GameUI/HUDTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(gui::HUDTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const gui::HUDTemplate& value)
{
}