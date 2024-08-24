#pragma once

#include "Math/Rotator.h"

namespace eng::settings
{
	struct Audio
	{
		int32 m_EffectVolume = 50;
		int32 m_MusicVolume = 50;
	};

	struct Camera
	{
		Rotator m_RotateSpeed = Rotator(0.2f);
		float m_TranslateSpeed = 1000.f;
		float m_ZoomSpeed = 3000.f;
	};

	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
	};

	/// \brief Shared settings for all local users that is saved to a config file.
	struct LocalComponent : public ecs::SingletonComponent<LocalComponent>
	{
		Audio m_Audio = {};
		Camera m_Camera = {};
	};

	/// \brief Shared settings broadcast from the host to all users that is saved to the savegame.
	struct HostComponent : public ecs::Component<HostComponent>
	{
	};

	/// \brief Unique settings for each local user that is saved to the savegame.
	struct UserComponent : public ecs::Component<UserComponent>
	{
	};
}