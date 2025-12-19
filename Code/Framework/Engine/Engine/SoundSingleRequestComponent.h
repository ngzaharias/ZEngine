#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct SingleRequestComponent final : public ecs::Component<SingleRequestComponent>
	{
		str::Guid m_Asset = {};
	};
}