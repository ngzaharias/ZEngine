#include "CameraPCH.h"
#include "SharedCamera/CameraMove3DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::camera::Move3DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const shared::camera::Move3DTemplate& value)
{ 
}
