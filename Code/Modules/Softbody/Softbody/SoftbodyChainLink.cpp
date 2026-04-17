#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyChainLink.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strDirection = "m_Direction";
	const str::StringView strPosition = "m_Position";
}

template<>
void Visitor::ReadCustom(softbody::ChainLink& value) const
{
	Read(strDirection, value.m_Direction, value.m_Direction);
	Read(strPosition, value.m_Position, value.m_Position);
}
template<>
void Visitor::WriteCustom(const softbody::ChainLink& value)
{
	Write(strDirection, value.m_Direction);
	Write(strPosition, value.m_Position);
}