#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct ExecuteComponent final : public ecs::Component<ExecuteComponent>
	{
		str::Name m_Ability = {};
	};
}