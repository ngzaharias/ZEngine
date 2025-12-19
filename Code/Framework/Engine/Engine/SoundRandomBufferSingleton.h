#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Singleton.h"

namespace eng::sound
{
	struct RandomBufferSingleton final : public ecs::Singleton<RandomBufferSingleton>
	{
		Array<str::Guid> m_Requests = {};
	};
}