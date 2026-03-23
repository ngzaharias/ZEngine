#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace tactics
{
	struct PawnAbilitiesComponent final : public ecs::Component
		, ecs::IsPrototype
	{
		Array<str::Name> m_Abilities = {};
	};
}