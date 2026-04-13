#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenGroupTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strObjects = "m_Objects";
	const str::StringView strRevealed = "m_Revealed";
}

template<>
void Visitor::ReadCustom(client::hidden::GroupTemplate& value) const
{
	Read(strObjects, value.m_Objects, value.m_Objects);
	Read(strRevealed, value.m_Revealed, value.m_Revealed);
}
template<>
void Visitor::WriteCustom(const client::hidden::GroupTemplate& value)
{
	Write(strObjects, value.m_Objects);
	Write(strRevealed, value.m_Revealed);
}