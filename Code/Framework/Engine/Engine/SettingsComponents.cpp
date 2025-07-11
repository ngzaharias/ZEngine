#include "EnginePCH.h"
#include "Engine/SettingsComponents.h"

#include "Core/Array.h"
#include "Engine/Visitor.h"
#include "Engine/WindowManager.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAreSoftbodyEnabled = "m_AreSoftbodyEnabled";
	const str::StringView strAudio = "m_Audio";
	const str::StringView strCamera = "m_Camera";
	const str::StringView strClearColour = "m_ClearColour";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strGraphics = "m_Graphics";
	const str::StringView strLevel = "m_Level";
	const str::StringView strMonitor = "m_Monitor";
	const str::StringView strMusicVolume = "m_MusicVolume";
	const str::StringView strRefreshRate = "m_RefreshRate";
	const str::StringView strResolution = "m_Resolution";
	const str::StringView strRotateSpeed = "m_RotateSpeed";
	const str::StringView strTranslateSpeed = "m_TranslateSpeed";
	const str::StringView strZoomAmount = "m_ZoomAmount";
	const str::StringView strZoomSpeed = "m_ZoomSpeed";
	const str::StringView strWindow = "m_Window";
	const str::StringView strWindowMode = "m_WindowMode";

	static str::String m_Scratch = {};

	str::String MonitorToString(const int32 index)
	{
		switch (index)
		{
		case 0: return "Primary";
		case 1: return "Secondary";
		}
		return std::format("Monitor {}", index + 1);
	}
}

template<>
void eng::Visitor::ReadCustom(eng::settings::DebugComponent& value) const
{
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Read(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Read(strIsEditorModeEnabled, value.m_IsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::DebugComponent& value)
{
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Write(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Write(strIsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_AreLinesEnabled", value.m_AreLinesEnabled);
	result |= Write("m_ArePhysicsEnabled", value.m_ArePhysicsEnabled);
	result |= Write("m_AreSoftbodyEnabled", value.m_AreSoftbodyEnabled);
	result |= Write("m_IsEditorModeEnabled", value.m_IsEditorModeEnabled);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LaunchComponent& value) const
{
	Read(strLevel, value.m_Level, value.m_Level);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LaunchComponent& value)
{
	Write(strLevel, value.m_Level);
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LocalComponent& value) const
{
	Read(strAudio, value.m_Audio, value.m_Audio);
	Read(strCamera, value.m_Camera, value.m_Camera);
	Read(strWindow, value.m_Window, value.m_Window);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LocalComponent& value)
{
	Write(strAudio, value.m_Audio);
	Write(strCamera, value.m_Camera);
	Write(strWindow, value.m_Window);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::LocalComponent& value)
{
	bool result = false;
	result |= Write("m_Audio", value.m_Audio);
	result |= Write("m_Camera", value.m_Camera);
	result |= Write("m_Window", value.m_Window);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Audio& value) const
{
	Read(strEffectVolume, value.m_EffectVolume, value.m_EffectVolume);
	Read(strMusicVolume, value.m_MusicVolume, value.m_MusicVolume);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Audio& value)
{
	Write(strEffectVolume, value.m_EffectVolume);
	Write(strMusicVolume, value.m_MusicVolume);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::Audio& value)
{
	bool result = false;
	result |= Write("m_EffectVolume", value.m_EffectVolume);
	result |= Write("m_MusicVolume", value.m_MusicVolume);
	value.m_EffectVolume = math::Clamp(value.m_EffectVolume, 0, 100);
	value.m_MusicVolume = math::Clamp(value.m_MusicVolume, 0, 100);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Camera& value) const
{
	Read(strRotateSpeed, value.m_RotateSpeed, value.m_RotateSpeed);
	Read(strTranslateSpeed, value.m_TranslateSpeed, value.m_TranslateSpeed);
	Read(strZoomAmount, value.m_ZoomAmount, value.m_ZoomAmount);
	Read(strZoomSpeed, value.m_ZoomSpeed, value.m_ZoomSpeed);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Camera& value)
{
	Write(strRotateSpeed, value.m_RotateSpeed);
	Write(strTranslateSpeed, value.m_TranslateSpeed);
	Write(strZoomAmount, value.m_ZoomAmount);
	Write(strZoomSpeed, value.m_ZoomSpeed);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::Camera& value)
{
	bool result = false;
	result |= Write("m_RotateSpeed", value.m_RotateSpeed);
	result |= Write("m_TranslateSpeed", value.m_TranslateSpeed);
	result |= Write("m_ZoomAmount", value.m_ZoomAmount);
	result |= Write("m_ZoomSpeed", value.m_ZoomSpeed);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Window& value) const
{
	Read(strWindowMode, value.m_WindowMode, value.m_WindowMode);
	Read(strResolution, value.m_Resolution, value.m_Resolution);
	Read(strRefreshRate, value.m_RefreshRate, value.m_RefreshRate);
	Read(strMonitor, value.m_Monitor, value.m_Monitor);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Window& value)
{
	Write(strWindowMode, value.m_WindowMode);
	Write(strResolution, value.m_Resolution);
	Write(strRefreshRate, value.m_RefreshRate);
	Write(strMonitor, value.m_Monitor);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::Window& value)
{
	bool result = false;

	const auto& windowManager = GetPayload<const eng::WindowManager>();
	const eng::Monitor* monitor = windowManager.GetMonitor(value.m_Monitor);
	if (!monitor)
		return false;

	ImGui::TableNextRow();
	if (WriteHeader("m_Monitor", value.m_Monitor))
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::BeginDisabled(value.m_WindowMode == eng::EWindowMode::Windowed);

		m_Scratch = MonitorToString(value.m_Monitor);
		if (ImGui::BeginCombo("##monitor", m_Scratch.c_str()))
		{
			for (const auto& [i, unused] : enumerate::Forward(windowManager.GetMonitors()))
			{
				m_Scratch = MonitorToString(i);
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					value.m_Monitor = i;
					result = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
	}

	ImGui::TableNextRow();
	if (WriteHeader("m_Mode", value.m_WindowMode))
	{
		ImGui::TableSetColumnIndex(1);

		m_Scratch = EnumToString(value.m_WindowMode);
		if (ImGui::BeginCombo("##mode", m_Scratch.c_str()))
		{
			for (const eng::EWindowMode mode : windowManager.GetWindowModes())
			{
				m_Scratch = EnumToString(mode);
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					value.m_WindowMode = mode;
					result = true;
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::TableNextRow();
	if (WriteHeader("m_Resolution", value.m_Resolution))
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::BeginDisabled(value.m_WindowMode != eng::EWindowMode::Windowed);

		m_Scratch = std::format("{}x{}", value.m_Resolution.x, value.m_Resolution.y);
		if (ImGui::BeginCombo("##resolution", m_Scratch.c_str()))
		{
			for (const Vector2u& resolution : windowManager.GetResolutions())
			{
				const bool isNative = resolution == monitor->m_Resolution;
				m_Scratch = std::format("{}x{}{}", resolution.x, resolution.y, isNative ? " (native)" : "");
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					result = true;
					value.m_Resolution = resolution;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
	}

	ImGui::TableNextRow();
	if (WriteHeader("m_RefreshRate", value.m_RefreshRate))
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::BeginDisabled(value.m_WindowMode != eng::EWindowMode::Fullscreen);

		m_Scratch = std::format("{}", value.m_RefreshRate);
		if (ImGui::BeginCombo("##refreshrate", m_Scratch.c_str()))
		{
			for (const int32 refreshRate : windowManager.GetRefreshRates())
			{
				const bool isNative = refreshRate == monitor->m_RefreshRate;
				m_Scratch = std::format("{}{}", refreshRate, isNative ? " (native)" : "");
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					result = true;
					value.m_RefreshRate = refreshRate;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
	}

	return result;
}
