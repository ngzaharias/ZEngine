#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenObjectTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strGroup = "m_Group";
}

template<>
void Visitor::ReadCustom(client::hidden::ObjectTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const client::hidden::ObjectTemplate& value)
{
}