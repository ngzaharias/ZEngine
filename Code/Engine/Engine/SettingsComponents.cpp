#include "EnginePCH.h"
#include "Engine/SettingsComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strAreHiddenEnabled = "m_AreHiddenEnabled";
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAudio = "m_Audio";
	const str::StringView strCamera = "m_Camera";
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strMusicVolume = "m_MusicVolume";
	const str::StringView strRotateSpeed = "m_RotateSpeed";
	const str::StringView strTranslateSpeed = "m_TranslateSpeed";
	const str::StringView strZoomSpeed = "m_ZoomSpeed";
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
void eng::Visitor::ReadCustom(eng::settings::Camera& value) const
{
	Read(strTranslateSpeed, value.m_TranslateSpeed, value.m_TranslateSpeed);
	Read(strRotateSpeed, value.m_RotateSpeed, value.m_RotateSpeed);
	Read(strZoomSpeed, value.m_ZoomSpeed, value.m_ZoomSpeed);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::Camera& value)
{
	Write(strTranslateSpeed, value.m_TranslateSpeed);
	Write(strRotateSpeed, value.m_RotateSpeed);
	Write(strZoomSpeed, value.m_ZoomSpeed);
}
