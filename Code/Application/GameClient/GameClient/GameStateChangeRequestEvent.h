#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "GameClient/GameStateTypes.h"

namespace gamestate
{
	/// \brief Holds a single gamestate Event.
	struct ChangeRequestEvent final : public ecs::Event<ChangeRequestEvent>
	{
		Array<gamestate::State> m_Queue = { };
	};
}