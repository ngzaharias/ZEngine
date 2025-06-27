#pragma once

#include "Core/String.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
	class System;

	using SystemId = int32;

	struct SystemEntry
	{
		int32 m_Priority = 0;
		ecs::System* m_System = nullptr;
		str::String m_Name = { };

		using Initialise = void(ecs::EntityWorld&, ecs::System&);
		using Shutdown = void(ecs::EntityWorld&, ecs::System&);
		using Update = void(ecs::EntityWorld&, ecs::System&, const GameTime&);
		Initialise* m_Initialise = nullptr;
		Shutdown* m_Shutdown = nullptr;
		Update* m_Update = nullptr;
	};
}