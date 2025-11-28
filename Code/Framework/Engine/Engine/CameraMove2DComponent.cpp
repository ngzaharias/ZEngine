#include "EnginePCH.h"
#include "Engine/CameraMove2DComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

template<>
void eng::Visitor::ReadCustom(eng::camera::Move2DComponent& value) const { }
template<>
void eng::Visitor::WriteCustom(const eng::camera::Move2DComponent& value) { }
template<>
bool imgui::Inspector::WriteCustom(eng::camera::Move2DComponent& value) { return false; }