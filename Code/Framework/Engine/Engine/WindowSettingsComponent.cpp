#include "EnginePCH.h"
#include "Engine/WindowSettingsComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strMonitor = "m_Monitor";
	const str::StringView strRefreshRate = "m_RefreshRate";
	const str::StringView strResolution = "m_Resolution";
	const str::StringView strWindowMode = "m_WindowMode";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::WindowSingleton& value) const
{
	Read(strWindowMode, value.m_WindowMode, value.m_WindowMode);
	Read(strResolution, value.m_Resolution, value.m_Resolution);
	Read(strRefreshRate, value.m_RefreshRate, value.m_RefreshRate);
	Read(strMonitor, value.m_Monitor, value.m_Monitor);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::WindowSingleton& value)
{
	Write(strWindowMode, value.m_WindowMode);
	Write(strResolution, value.m_Resolution);
	Write(strRefreshRate, value.m_RefreshRate);
	Write(strMonitor, value.m_Monitor);
}