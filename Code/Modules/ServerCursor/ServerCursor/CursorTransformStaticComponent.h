#pragma once

#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "Network/PeerId.h"

namespace server::cursor
{
	struct TransformStaticComponent final : public ecs::StaticComponent
	{
		Map<net::PeerId, ecs::Entity> m_Peers = {};
	};
}