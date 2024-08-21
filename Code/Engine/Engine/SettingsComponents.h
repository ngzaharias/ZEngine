#pragma once

namespace eng::settings
{
	/// \brief Config settings that is specific to the app and not the game.
	struct ConfigComponent : public ecs::SingletonComponent<ConfigComponent>
	{
	};

	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
	};

	/// \brief Shared settings for all local users that is saved to a config file.
	struct LocalComponent : public ecs::SingletonComponent<LocalComponent>
	{
		int32 m_EffectVolume = 50;
		int32 m_MusicVolume = 50;
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