#include "GameUIPCH.h"
#include "GameUI/VMWindowMode.h"

#include "Core/EnumHelpers.h"
#include "Core/StringView.h"

#include <NsCore/ReflectionImplement.h>

gui::VMWindowMode::VMWindowMode(const eng::EWindowMode windowMode)
	: m_WindowMode(windowMode)
{
	m_Name = EnumToString(m_WindowMode);
}

const char* gui::VMWindowMode::GetName() const
{
	return m_Name.c_str();
}

NS_IMPLEMENT_REFLECTION(gui::VMWindowMode)
{
	NsProp("Name", &gui::VMWindowMode::GetName);
}