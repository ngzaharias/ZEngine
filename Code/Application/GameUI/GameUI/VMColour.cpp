#include "GameUIPCH.h"
#include "GameUI/VMColour.h"

#include "Core/Name.h"

#include <NsCore/ReflectionImplement.h>
#include <NsCore/StringFwd.h>

gui::VMColour::VMColour(const str::Name& name, const str::Guid& guid)
	: m_Name(name)
	, m_Guid(guid)
{
}

const char* gui::VMColour::GetName() const
{
	return m_Name.ToChar();
}

NS_IMPLEMENT_REFLECTION(gui::VMColour)
{
	NsProp("Name", &gui::VMColour::GetName);
}