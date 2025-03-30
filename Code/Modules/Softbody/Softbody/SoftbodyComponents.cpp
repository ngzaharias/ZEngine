#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strData = "m_Data";
}

template<>
void eng::Visitor::ReadCustom(softbody::ChainComponent& value) const
{
}
template<>
void eng::Visitor::WriteCustom(const softbody::ChainComponent& value)
{
}