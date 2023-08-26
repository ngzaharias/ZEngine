#pragma once

#include <Core/String.h>

class GameTime;

namespace ecs
{
	class EntityWorld;
	class System;

	using SystemId = int32;

	struct SystemEntry
	{
		ecs::System* m_System = nullptr;

		int32 m_Priority = 0;
		SystemId m_SystemId = INT32_MAX;
		str::String m_DebugName = { };

		using Initialise = void(ecs::EntityWorld&, ecs::System&);
		using Shutdown = void(ecs::EntityWorld&, ecs::System&);
		using Update = void(ecs::EntityWorld&, ecs::System&, const GameTime&);
		Initialise* m_Initialise = nullptr;
		Shutdown* m_Shutdown = nullptr;
		Update* m_Update = nullptr;
	};
}