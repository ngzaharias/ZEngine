#pragma once

#include "Core/Set.h"
#include "ECS/Component.h"
#include "Network/PeerId.h"

namespace ecs
{
	/// \brief Component added to host entities that are to be replicated to peers.
	struct ReplicationComponent final : public ecs::Component<ReplicationComponent>
	{
		Set<net::PeerId> m_Peers = {};
	};
}