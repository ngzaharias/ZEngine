#pragma once

class GameTime;

namespace ecs
{
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
