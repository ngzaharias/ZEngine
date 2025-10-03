#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace tactics
{
	struct SkillsComponent : ecs::Component<SkillsComponent>
	{
		Array<str::Guid> m_Skills = {};
	};
}