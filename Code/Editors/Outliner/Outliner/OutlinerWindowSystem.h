#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::outliner
{
	struct OpenWindowEvent;
	struct SelectSingleton;
	struct WindowComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::outliner
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::outliner::SelectSingleton,
			editor::outliner::WindowComponent>
			::Read<
			ecs::NameComponent,
			editor::outliner::OpenWindowEvent,
			eng::level::EntityComponent,
			gamestate::EditorComponent>;

		WindowSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
