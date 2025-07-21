#include "GameUIPCH.h"
#include "GameUI/VMResolution.h"

#include <NsCore/ReflectionImplement.h>

gui::VMResolution::VMResolution(const Vector2u& resolution, bool isNative)
	: m_Resolution(resolution)
{
	const str::String string = std::format("{}x{}{}", resolution.x, resolution.y, isNative ? " (native)" : "");
	m_Name = Noesis::String(string.data(), (uint32)string.size());
}

const char* gui::VMResolution::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMResolution)
{
	NsProp("Name", &gui::VMResolution::GetName);
}