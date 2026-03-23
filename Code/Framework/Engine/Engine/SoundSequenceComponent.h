#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct SequenceComponent final : public ecs::Component
	{
		str::Guid m_Asset = {};
		int32 m_Index = -1;
	};
}