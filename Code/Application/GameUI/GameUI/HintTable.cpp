#include "GameUIPCH.h"
#include "GameUI/HintTable.h"

#include "Core/Random.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strText = "m_Text";
	const gui::Hint s_Hint = { "<unknown>" };
}

const gui::Hint& gui::HintTable::GetRandom() const 
{
	const uint32 count = m_ObjectMap.GetCount();
	if (count == 0)
		return s_Hint;

	const uint32 index = random::Range<uint32>(0, count);
	return m_ObjectMap.Get(index);
}

template<>
void eng::Visitor::ReadCustom(gui::Hint& value) const
{
	Read(strText, value.m_Text, value.m_Text);
}