#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Singleton.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "GameClient/GameStateTypes.h"
#include "Network/Config.h"

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

	/// \brief Holds a single gamestate request.
	struct ChangeRequest final : public ecs::Event<ChangeRequest>
	{
		Array<gamestate::State> m_Queue = { };
	};

	/// \brief Signals that the current gamestate has finished.
	struct ChangeFinished final : public ecs::Event<ChangeFinished> { };

	/// \brief Holds the current gamestate as well as the current queue.
	struct StateSingleton final : public ecs::Singleton<StateSingleton>
	{
		gamestate::State m_State = gamestate::None();
		Array<gamestate::State> m_Queue = { };
	};
}