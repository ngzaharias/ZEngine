#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "GameClient/GameStateTypes.h"

namespace gamestate
{
	/// \brief Holds the current gamestate as well as the current queue.
	struct StateComponent final : public ecs::StaticComponent
	{
		gamestate::State m_State = gamestate::None();
		Array<gamestate::State> m_Queue = { };
	};
}