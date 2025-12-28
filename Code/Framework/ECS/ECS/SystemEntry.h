#pragma once

#include "Core/Set.h"
#include "Core/TypeInfo.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
	class System;
}

namespace ecs
{
	struct SystemEntry
	{
		using Initialise = void(ecs::EntityWorld&, ecs::System&);
		using Shutdown = void(ecs::EntityWorld&, ecs::System&);
		using Update = void(ecs::EntityWorld&, ecs::System&, const GameTime&);
		
		ecs::System* m_System = nullptr;
		Initialise* m_Initialise = nullptr;
		Shutdown* m_Shutdown = nullptr;
		Update* m_Update = nullptr;

		TypeId m_TypeId = -1;
		Set<TypeId> m_Read = {};
		Set<TypeId> m_Write = {};
	};
}