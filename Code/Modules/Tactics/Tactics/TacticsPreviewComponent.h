#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace tactics
{
	struct PreviewComponent final : public ecs::Component<PreviewComponent>
	{
		str::Name m_Ability = {};
	};
}