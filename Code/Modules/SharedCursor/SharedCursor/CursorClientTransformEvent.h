#pragma once

#include "ECS/Event.h"
#include "ECS/IsReplicated.h"
#include "Math/Vector.h"
#include "Network/PeerId.h"

namespace shared::cursor
{
	struct ClientTransformEvent final : public ecs::Event<ClientTransformEvent>
		, ecs::IsReplicated
	{
		net::PeerId m_PeerId = {};
		Vector2f m_Translate = {};
	};
}