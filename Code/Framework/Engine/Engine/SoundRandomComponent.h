#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct RandomComponent final : public ecs::Component<RandomComponent>
	{
		str::Guid m_Asset = {};
		int32 m_Index = 0;
	};
}