#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct AbilityComponent final : public ecs::Component<AbilityComponent>
	{
		Array<str::Name> m_Abilities = {};
	};
}