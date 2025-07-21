#include "GameUIPCH.h"
#include "GameUI/VMRefreshRate.h"

#include <NsCore/ReflectionImplement.h>

gui::VMRefreshRate::VMRefreshRate(const int32 refreshRate, bool isNative)
	: m_RefreshRate(refreshRate)
{
	const str::String string = std::format("{}{}", m_RefreshRate, isNative ? " (native)" : "");
	m_Name = Noesis::String(string.data(), (uint32)string.size());
}

const char* gui::VMRefreshRate::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMRefreshRate)
{
	NsProp("Name", &gui::VMRefreshRate::GetName);
}