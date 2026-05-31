#pragma once

#include "ECS/Component.h"
#include "SharedSoftbody/SoftbodyChainLink.h"

namespace shared::softbody
{
	struct ChainComponent final : public ecs::Component
	{
		Array<ChainLink> m_Links = {};
	};
}