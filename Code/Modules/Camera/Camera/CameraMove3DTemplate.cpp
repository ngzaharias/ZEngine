#include "CameraPCH.h"
#include "Camera/CameraMove3DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(camera::Move3DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const camera::Move3DTemplate& value)
{ 
}
