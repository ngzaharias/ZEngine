#pragma once

#include "Core/Array.h"
#include "ECS/Singleton.h"
#include "GameClient/GameStateTypes.h"

namespace gamestate
{
	/// \brief Holds the current gamestate as well as the current queue.
	struct StateSingleton final : public ecs::Singleton<StateSingleton>
	{
		gamestate::State m_State = gamestate::None();
		Array<gamestate::State> m_Queue = { };
	};
}