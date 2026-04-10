#include "EnginePCH.h"
#include "Engine/CameraTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strProjection = "m_Projection";
}

template<>
void Visitor::ReadCustom(eng::CameraTemplate& value) const
{
	Read(strProjection, value.m_Projection, value.m_Projection);
}
template<>
void Visitor::WriteCustom(const eng::CameraTemplate& value)
{
	Write(strProjection, value.m_Projection);
}