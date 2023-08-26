#pragma once

class GameTime;

#include <ECS/Forward.h>

namespace ecs
{
	class EntityWorld;

	class System
	{
	public:
		virtual ~System() = default;

		virtual void Initialise() { }
		virtual void Shutdown() { }

		virtual void Update(const GameTime& gameTime) { }
	};

	struct SystemTag {};
}
