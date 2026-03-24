#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Network/PeerId.h"

namespace server::cursor
{
	struct TransformStaticComponent final : public ecs::StaticComponent
	{
		Map<net::PeerId, ecs::Entity> m_Peers = {};
	};
}