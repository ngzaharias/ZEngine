#include "CameraPCH.h"
#include "Camera/CameraMove3DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(camera::Move3DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const camera::Move3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(camera::Move3DComponent& value) { return false; }