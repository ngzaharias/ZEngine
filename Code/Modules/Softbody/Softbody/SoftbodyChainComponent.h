#pragma once

#include "ECS/Component.h"
#include "Softbody/SoftbodyChainLink.h"

namespace softbody
{
	struct ChainComponent final : public ecs::Component
	{
		Array<ChainLink> m_Links = {};
	};
}