#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "GameClient/GameStateTypes.h"

namespace gamestate
{
	/// \brief Holds a single gamestate request.
	struct ChangeRequest final : public ecs::Event<ChangeRequest>
	{
		Array<gamestate::State> m_Queue = { };
	};
}