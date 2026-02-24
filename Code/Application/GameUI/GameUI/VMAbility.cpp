#include "GameUIPCH.h"
#include "GameUI/VMAbility.h"

#include <NsCore/ReflectionImplement.h>
#include <NsCore/StringFwd.h>

gui::VMAbility::VMAbility(const str::Name& name)
	: m_Name(name.ToChar())
	, m_Key(name)
{
}

const char* gui::VMAbility::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMAbility)
{
	NsProp("Name", &gui::VMAbility::GetName);
}