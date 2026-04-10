#include "CameraPCH.h"
#include "Camera/CameraPan3DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(camera::Pan3DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const camera::Pan3DTemplate& value)
{ 
}
