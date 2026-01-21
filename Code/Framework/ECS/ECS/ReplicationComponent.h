#pragma once

#include "ECS/Component.h"

namespace ecs
{
	/// \brief Component added to host entities that are to be replicated to peers.
	struct ReplicationComponent final : public ecs::Component<ReplicationComponent>
	{
	};
}