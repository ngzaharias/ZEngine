#pragma once

#include "ECS/Component.h"
#include "ECS/IsReplicated.h"
#include "Math/Vector.h"
#include "Network/PeerId.h"

namespace shared::cursor
{
	struct TransformComponent final : public ecs::Component<TransformComponent>
		, ecs::IsReplicated
	{
		net::PeerId m_PeerId = {};
		Vector3f m_Translate = {};
	};
}