#include "CameraPCH.h"
#include "SharedCamera/CameraPan3DTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::camera::Pan3DTemplate& value) const
{ 
}
template<>
void Visitor::WriteCustom(const shared::camera::Pan3DTemplate& value)
{ 
}
