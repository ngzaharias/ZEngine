#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "GameClient/GameStateTypes.h"

namespace shared::gamestate
{
	/// \brief Holds a single gamestate Event.
	struct ChangeRequestEvent final : public ecs::Event
	{
		Array<shared::gamestate::State> m_Queue = { };
	};
}