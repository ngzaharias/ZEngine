#pragma once

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	class GameDebug final
	{
	public:
		GameDebug(
			ecs::EntityWorld& clientWorld,
			ecs::EntityWorld& serverWorld);

		void Register();

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
	};
}