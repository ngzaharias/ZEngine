#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::spell
{
	struct GraphComponent;
	struct LinkCreateEvent;
	struct LinkDestroyEvent;
	struct NodeCreateEvent;
	struct WindowComponent;
	struct WindowEvent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::spell
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::spell::LinkCreateEvent,
			editor::spell::LinkDestroyEvent,
			editor::spell::NodeCreateEvent,
			editor::spell::WindowComponent>
			::Read<
			editor::spell::GraphComponent,
			editor::spell::WindowEvent,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};