#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "GameClient/GameStateTypes.h"

namespace shared::gamestate
{
	/// \brief Holds the current gamestate as well as the current queue.
	struct StateComponent final : public ecs::StaticComponent
	{
		shared::gamestate::State m_State = shared::gamestate::None();
		Array<shared::gamestate::State> m_Queue = { };
	};
}