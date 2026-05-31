#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace shared::tactics
{
	struct AbilityPreviewComponent final : public ecs::Component
	{
		str::Name m_Ability = {};
	};
}