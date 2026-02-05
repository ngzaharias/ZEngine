#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace tactics
{
	struct AbilityComponent final : public ecs::Component<AbilityComponent>
		, ecs::IsPrototype
	{
		Array<str::Name> m_Abilities = {};
	};
}