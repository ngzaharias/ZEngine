#include "GameUIPCH.h"
#include "GameUI/VMRefreshRate.h"

#include <NsCore/ReflectionImplement.h>

gui::VMRefreshRate::VMRefreshRate(const int32 refreshRate, bool isNative)
	: m_RefreshRate(refreshRate)
{
	m_Name = std::format("{}{}", m_RefreshRate, isNative ? " (native)" : "");
}

const char* gui::VMRefreshRate::GetName() const
{
	return m_Name.c_str();
}

NS_IMPLEMENT_REFLECTION(gui::VMRefreshRate)
{
	NsProp("Name", &gui::VMRefreshRate::GetName);
}