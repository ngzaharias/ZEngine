#include "EnginePCH.h"
#include "Engine/AudioSettingsComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strEffectVolume = "m_EffectVolume";
	const str::StringView strMasterVolume = "m_MasterVolume";
	const str::StringView strMusicVolume = "m_MusicVolume";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::AudioSingleton& value) const
{
	Read(strEffectVolume, value.m_EffectVolume, value.m_EffectVolume);
	Read(strMasterVolume, value.m_MasterVolume, value.m_MasterVolume);
	Read(strMusicVolume, value.m_MusicVolume, value.m_MusicVolume);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::AudioSingleton& value)
{
	Write(strEffectVolume, value.m_EffectVolume);
	Write(strMasterVolume, value.m_MasterVolume);
	Write(strMusicVolume, value.m_MusicVolume);
}