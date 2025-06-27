#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "GameClient/GameStateTypes.h"
#include "Network/Config.h"

namespace gamestate
{
	/// \brief Holds the state for joining a server.
	struct NetworkJoinComponent : public ecs::SingletonComponent<NetworkJoinComponent> 
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
	struct RequestComponent : public ecs::EventComponent<RequestComponent>
	{
		Array<gamestate::State> m_Queue = { };
	};

	/// \brief Holds the current gamestate as well as the current queue.
	struct StateComponent : public ecs::SingletonComponent<StateComponent>
	{
		gamestate::State m_State = gamestate::None();
		Array<gamestate::State> m_Queue = { };
	};

	/// \brief Signals that the current gamestate has finished.
	struct StateFinishedComponent : public ecs::EventComponent<StateFinishedComponent> { };
}