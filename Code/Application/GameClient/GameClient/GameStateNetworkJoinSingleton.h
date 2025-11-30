#pragma once

#include "ECS/Singleton.h"

namespace gamestate
{
	/// \brief Holds the state for joining a server.
	struct NetworkJoinSingleton final : public ecs::Singleton<NetworkJoinSingleton>
	{ 
		enum class EResult
		{
			Failure,
			Success,
		};

		enum class EState
		{
			None,
			Connect,
			LoadLevel,
			SyncWorld,
			Finished,
		};

		EResult m_Result = EResult::Failure;
		EState m_State = EState::None;
	};
}