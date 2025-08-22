#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"
#include "Engine/WindowModeEnum.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

namespace eng::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
		bool m_AreLinesEnabled = false;
		bool m_ArePhysicsEnabled = false;
		bool m_AreSoftbodyEnabled = false;
		bool m_IsEditorModeEnabled = false;
	};

	/// \brief Launch settings for the game.
	struct LaunchComponent : public ecs::SingletonComponent<LaunchComponent>
	{
		// the level that is loaded when the game first launches
		str::Name m_Level = {};
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