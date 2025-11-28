#include "EnginePCH.h"
#include "Engine/CameraMove3DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(eng::camera::Move3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Move3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Move3DComponent& value) { return false; }