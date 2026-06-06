#pragma once

#include "Core/Array.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/IsReplicated.h"

namespace shared::tactics
{
	struct PawnAbilitiesTemplate final : public ecs::TemplateComponent
		, ecs::IsReplicated
	{
		Array<str::Name> m_Abilities = {};
	};
}