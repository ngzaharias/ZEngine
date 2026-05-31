#include "CameraPCH.h"
#include "SharedCamera/CameraBound2DTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strMax = "m_Max";
	const str::StringView strMin = "m_Min";
}

template<>
void Visitor::ReadCustom(shared::camera::Bound2DTemplate& value) const
{
	Read(strMax, value.m_Max, value.m_Max);
	Read(strMin, value.m_Min, value.m_Min);
}
template<>
void Visitor::WriteCustom(const shared::camera::Bound2DTemplate& value)
{ 
	Write(strMax, value.m_Max);
	Write(strMin, value.m_Min);
}