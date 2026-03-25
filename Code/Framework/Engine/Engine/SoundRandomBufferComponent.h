#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::sound
{
	struct RandomBufferComponent final : public ecs::StaticComponent
	{
		Array<str::Guid> m_Requests = {};
	};
}