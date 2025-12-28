#include "CameraPCH.h"
#include "Camera/CameraMove2DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(camera::Move2DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const camera::Move2DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(camera::Move2DComponent& value) { return false; }