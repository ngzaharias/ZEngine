#include "CameraPCH.h"
#include "Camera/CameraMove3DComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(camera::Move3DComponent& value) const { }
template<>
void Visitor::WriteCustom(const camera::Move3DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(camera::Move3DComponent& value) { return false; }