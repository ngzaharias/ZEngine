#pragma once

#include "Core/Guid.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng
{
	struct PrototypeComponent : public ecs::Component<PrototypeComponent>
	{
		str::Guid m_Guid = {};
		str::Path m_Path = { };
	};
}