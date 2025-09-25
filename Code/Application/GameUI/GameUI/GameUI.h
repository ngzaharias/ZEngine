#pragma once

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	class PrototypeManager;
	class TableHeadmaster;
	class UIManager;
	class WindowManager;
}

namespace gui
{
	struct Dependencies
	{
		eng::PrototypeManager& m_PrototypeManager;
		eng::TableHeadmaster& m_Headmaster;
		eng::UIManager& m_UIManager;
		eng::WindowManager& m_WindowManager;
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