#pragma once

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace gui
{
	struct Dependencies
	{
	};

	class GameUI final
	{
	public:
		GameUI(ecs::EntityWorld& entityWorld);

		void Register(const Dependencies& dependencies);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_EntityWorld;
	};
}