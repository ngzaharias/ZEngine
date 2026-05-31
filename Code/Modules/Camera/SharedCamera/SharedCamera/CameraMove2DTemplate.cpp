#include "CameraPCH.h"
#include "SharedCamera/CameraMove2DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::camera::Move2DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const shared::camera::Move2DTemplate& value)
{ 
}
