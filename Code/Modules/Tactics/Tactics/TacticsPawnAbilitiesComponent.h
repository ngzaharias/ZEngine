#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct PawnAbilitiesComponent final : public ecs::Component
	{
		Array<str::Name> m_Abilities = {};
	};
}