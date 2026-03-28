#include "CameraPCH.h"
#include "Camera/CameraMove2DComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

template<>
void Visitor::ReadCustom(camera::Move2DComponent& value) const { }
template<>
void Visitor::WriteCustom(const camera::Move2DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(camera::Move2DComponent& value) { return false; }