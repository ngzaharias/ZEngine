#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct PawnAbilitiesTemplate final : public ecs::TemplateComponent
	{
		Array<str::Name> m_Abilities = {};
	};
}