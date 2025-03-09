#include "EnginePCH.h"
#include "Engine/SettingsComponents.h"

#include "Core/Array.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAudio = "m_Audio";
	const str::StringView strCamera = "m_Camera";
	const str::StringView strClearColour = "m_ClearColour";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strGraphics = "m_Graphics";
	const str::StringView strLevel = "m_Level";
	const str::StringView strMode = "m_Mode";
	const str::StringView strMusicVolume = "m_MusicVolume";
	const str::StringView strResolution = "m_Resolution";
	const str::StringView strRotateSpeed = "m_RotateSpeed";
	const str::StringView strTranslateSpeed = "m_TranslateSpeed";
	const str::StringView strZoomAmount = "m_ZoomAmount";

	constexpr Vector2u s_Resolutions[] = {
		Vector2u(800, 600),
		Vector2u(1600, 1024),
		Vector2u(1920, 1080),
		Vector2u(2560, 1440) };

	static str::String m_Scratch = {};
}

template<>
void eng::Visitor::ReadCustom(eng::settings::DebugComponent& value) const
{
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::DebugComponent& value)
{
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_AreLinesEnabled", value.m_AreLinesEnabled);
	result |= Write("m_ArePhysicsEnabled", value.m_ArePhysicsEnabled);
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
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LocalComponent& value)
{
	Write(strAudio, value.m_Audio);
	Write(strCamera, value.m_Camera);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::LocalComponent& value)
{
	bool result = false;
	result |= Write("m_Audio", value.m_Audio);
	result |= Write("m_Camera", value.m_Camera);
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
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Camera& value)
{
	Write(strRotateSpeed, value.m_RotateSpeed);
	Write(strTranslateSpeed, value.m_TranslateSpeed);
	Write(strZoomAmount, value.m_ZoomAmount);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::Camera& value)
{
	bool result = false;
	result |= Write("m_RotateSpeed", value.m_RotateSpeed);
	result |= Write("m_TranslateSpeed", value.m_TranslateSpeed);
	result |= Write("m_ZoomAmount", value.m_ZoomAmount);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::GraphicsComponent& value) const
{
	Read(strMode, value.m_Mode, value.m_Mode);
	Read(strResolution, value.m_Resolution, value.m_Resolution);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::GraphicsComponent& value)
{
	Write(strMode, value.m_Mode);
	Write(strResolution, value.m_Resolution);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::GraphicsComponent& value)
{
	bool result = false;

	if (WriteHeader("m_Mode", value.m_Mode))
	{
		ImGui::TableSetColumnIndex(1);

		m_Scratch = EnumToString(value.m_Mode);
		if (ImGui::BeginCombo("##mode", m_Scratch.c_str()))
		{
			constexpr int32 count = EnumToCount<eng::settings::EMode>();
			for (int32 i = 0; i < count; ++i)
			{
				const auto mode = IndexToEnum<eng::settings::EMode>(i);
				m_Scratch = EnumToString(mode);
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					result = true;
					value.m_Mode = mode;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::TableNextRow();

	if (WriteHeader("m_Resolution", value.m_Resolution))
	{
		ImGui::TableSetColumnIndex(1);

		m_Scratch = std::format("{}x{}", value.m_Resolution.x, value.m_Resolution.y);
		if (ImGui::BeginCombo("##resolution", m_Scratch.c_str()))
		{
			constexpr int32 count = std::extent<decltype(s_Resolutions)>::value;
			for (int32 i = 0; i < count; ++i)
			{
				const Vector2u& resolution = s_Resolutions[i];
				m_Scratch = std::format("{}x{}", resolution.x, resolution.y);
				if (ImGui::Selectable(m_Scratch.c_str()))
				{
					result = true;
					value.m_Resolution = resolution;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::TableNextRow();

	return result;
}
