#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct AbilityComponent : ecs::Component<AbilityComponent>
	{
		Array<str::Name> m_Abilities = {};
	};
}