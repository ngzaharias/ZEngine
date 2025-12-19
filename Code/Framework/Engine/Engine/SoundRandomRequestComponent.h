#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct RandomRequestComponent final : public ecs::Component<RandomRequestComponent>
	{
		str::Guid m_Asset = {};
	};
}