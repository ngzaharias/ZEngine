#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyChainComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAngle = "m_Angle";
	const str::StringView strGravity = "m_Gravity";
	const str::StringView strRadius = "m_Radius";
}

template<>
void eng::Visitor::ReadCustom(softbody::ChainComponent& value) const
{
	Read(strAngle, value.m_Angle, value.m_Angle);
	Read(strRadius, value.m_Radius, value.m_Radius);
	Read(strGravity, value.m_Gravity, value.m_Gravity);

	value.m_Angle = math::Clamp(value.m_Angle, 0.f, 180.f);
	value.m_Radius = math::Max(value.m_Radius, 0.f);
}
template<>
void eng::Visitor::WriteCustom(const softbody::ChainComponent& value)
{
	Write(strAngle, value.m_Angle);
	Write(strRadius, value.m_Radius);
	Write(strGravity, value.m_Gravity);
}
template<>
bool imgui::Inspector::WriteCustom(softbody::ChainComponent& value)
{
	bool result = false;
	result |= Write("m_Angle", value.m_Angle);
	result |= Write("m_Radius", value.m_Radius);
	result |= Write("m_Gravity", value.m_Gravity);

	value.m_Angle = math::Clamp(value.m_Angle, 0.f, 180.f);
	value.m_Radius = math::Max(value.m_Radius, 0.f);
	return result;
}