#include "EnginePCH.h"
#include "Engine/CameraPan3DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(eng::camera::Pan3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Pan3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Pan3DComponent& value) { return false; }
