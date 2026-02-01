#pragma once

#include "ECS/Component.h"
#include "Network/PeerId.h"

namespace client::network
{
	/// \brief
	struct PeerComponent final : public ecs::Component<PeerComponent>
	{
		net::PeerId m_PeerId = {};
	};
}