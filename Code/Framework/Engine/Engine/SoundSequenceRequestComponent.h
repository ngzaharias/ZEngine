#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct SequenceRequestComponent final : public ecs::Component<SequenceRequestComponent>
	{
		str::Guid m_Asset = {};
	};
}