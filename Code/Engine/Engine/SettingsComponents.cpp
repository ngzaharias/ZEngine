#include "EnginePCH.h"
#include "Engine/SettingsComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreHiddenEnabled = "m_AreHiddenEnabled";
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAudio = "m_Audio";
	const str::StringView strCamera = "m_Camera";
	const str::StringView strClearColour = "m_ClearColour";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strGraphics = "m_Graphics";
	const str::StringView strMusicVolume = "m_MusicVolume";
	const str::StringView strRotateSpeed = "m_RotateSpeed";
	const str::StringView strTranslateSpeed = "m_TranslateSpeed";
	const str::StringView strZoomAmount = "m_ZoomAmount";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::DebugComponent& value) const
{
	Read(strAreHiddenEnabled, value.m_AreHiddenEnabled, value.m_AreHiddenEnabled);
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::DebugComponent& value)
{
	Write(strAreHiddenEnabled, value.m_AreHiddenEnabled);
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_AreHiddenEnabled", value.m_AreHiddenEnabled);
	result |= Write("m_AreLinesEnabled", value.m_AreLinesEnabled);
	result |= Write("m_ArePhysicsEnabled", value.m_ArePhysicsEnabled);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LocalComponent& value) const
{
	Read(strAudio, value.m_Audio, value.m_Audio);
	Read(strCamera, value.m_Camera, value.m_Camera);
	Read(strGraphics, value.m_Graphics, value.m_Graphics);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LocalComponent& value)
{
	Write(strAudio, value.m_Audio);
	Write(strCamera, value.m_Camera);
	Write(strGraphics, value.m_Graphics);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::LocalComponent& value)
{
	bool result = false;
	result |= Write("m_Audio", value.m_Audio);
	result |= Write("m_Camera", value.m_Camera);
	result |= Write("m_Graphics", value.m_Graphics);
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
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::settings::Camera& value) const
{
	Read(strTranslateSpeed, value.m_TranslateSpeed, value.m_TranslateSpeed);
	Read(strRotateSpeed, value.m_RotateSpeed, value.m_RotateSpeed);
	Read(strZoomAmount, value.m_ZoomAmount, value.m_ZoomAmount);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Camera& value)
{
	Write(strTranslateSpeed, value.m_TranslateSpeed);
	Write(strRotateSpeed, value.m_RotateSpeed);
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
void eng::Visitor::ReadCustom(eng::settings::Graphics& value) const
{
	Read(strClearColour, value.m_ClearColour, value.m_ClearColour);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Graphics& value)
{
	Write(strClearColour, value.m_ClearColour);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::Graphics& value)
{
	bool result = false;
	result |= Write("m_ClearColour", value.m_ClearColour);
	return result;
}
