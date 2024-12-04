#pragma once

#include "ECS/Component.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

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
		float m_ZoomAmount = 100.f;
	};

	struct Graphics
	{
		Vector3f m_ClearColour = Vector3f(0.24f);
	};

	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
		bool m_AreLinesEnabled = false;
		bool m_ArePhysicsEnabled = false;
	};

	/// \brief Shared settings for all local users that is saved to a config file.
	struct LocalComponent : public ecs::SingletonComponent<LocalComponent>
	{
		Audio m_Audio = {};
		Camera m_Camera = {};
		Graphics m_Graphics = {};
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