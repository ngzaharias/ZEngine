#pragma once

#include "ECS/Entity.h"
#include "ECS/Singleton.h"
#include "Network/PeerId.h"

namespace server::cursor
{
	struct TransformSingleton final : public ecs::Singleton
	{
		Map<net::PeerId, ecs::Entity> m_Peers = {};
	};
}