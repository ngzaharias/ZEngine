#pragma once

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

		Set<TypeId> m_DependencyRead = {};
		Set<TypeId> m_DependencyWrite = {};
	};
}