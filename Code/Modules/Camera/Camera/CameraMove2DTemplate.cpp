#include "CameraPCH.h"
#include "Camera/CameraMove2DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(camera::Move2DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const camera::Move2DTemplate& value)
{ 
}
