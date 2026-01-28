#include "GameUIPCH.h"
#include "GameUI/VMWindowMode.h"

#include "Core/EnumHelpers.h"
#include "Core/StringView.h"

#include <NsCore/ReflectionImplement.h>

gui::VMWindowMode::VMWindowMode(const eng::EWindowMode windowMode)
	: m_WindowMode(windowMode)
{
	const str::StringView string = EnumToString(m_WindowMode);
	m_Name = Noesis::String(string.data(), (uint32)string.size());
}

const char* gui::VMWindowMode::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMWindowMode)
{
	NsProp("Name", &gui::VMWindowMode::GetName);
}