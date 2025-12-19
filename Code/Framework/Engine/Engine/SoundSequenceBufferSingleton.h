#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Singleton.h"

namespace eng::sound
{
	struct SequenceBufferSingleton final : public ecs::Singleton<SequenceBufferSingleton>
	{
		Array<str::Guid> m_Requests = {};
	};
}