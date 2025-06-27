#pragma once

#include "Core/Guid.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng
{
	struct PrototypeComponent : public ecs::Component<PrototypeComponent>
	{
		str::Guid m_Guid = {};
		// #todo: remove path and instead replace it with the level and 
		// saving can then reference the level path instead.
		str::Path m_Path = { };
	};
}