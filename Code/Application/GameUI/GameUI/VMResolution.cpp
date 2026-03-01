#include "GameUIPCH.h"
#include "GameUI/VMResolution.h"

#include <NsCore/ReflectionImplement.h>

gui::VMResolution::VMResolution(const Vector2u& resolution, bool isNative)
	: m_Resolution(resolution)
{
	m_Name = std::format("{}x{}{}", resolution.x, resolution.y, isNative ? " (native)" : "");
}

const char* gui::VMResolution::GetName() const
{
	return m_Name.c_str();
}

NS_IMPLEMENT_REFLECTION(gui::VMResolution)
{
	NsProp("Name", &gui::VMResolution::GetName);
}