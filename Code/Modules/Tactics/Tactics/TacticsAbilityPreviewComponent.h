#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct AbilityPreviewComponent final : public ecs::Component<AbilityPreviewComponent>
	{
		str::Name m_Ability = {};
	};
}