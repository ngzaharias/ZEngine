#include "GameUIPCH.h"
#include "GameUI/VMTheme.h"

#include "Core/Name.h"

#include <NsCore/ReflectionImplement.h>

gui::VMTheme::VMTheme(const str::Name& name, const str::Guid& guid)
	: m_Name(name)
	, m_Guid(guid)
{
}

const char* gui::VMTheme::GetName() const
{
	return m_Name.ToChar();
}

NS_IMPLEMENT_REFLECTION(gui::VMTheme)
{
	NsProp("Name", &gui::VMTheme::GetName);
}