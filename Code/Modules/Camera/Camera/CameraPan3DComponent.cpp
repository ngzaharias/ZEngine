#include "CameraPCH.h"
#include "Camera/CameraPan3DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(camera::Pan3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const camera::Pan3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(camera::Pan3DComponent& value) { return false; }
