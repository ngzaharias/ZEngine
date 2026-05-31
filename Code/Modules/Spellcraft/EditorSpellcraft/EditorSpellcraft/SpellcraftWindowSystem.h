#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::spellcraft
{
	struct GraphComponent;
	struct LinkCreateEvent;
	struct LinkDestroyEvent;
	struct NodeCreateEvent;
	struct WindowComponent;
	struct WindowEvent;
}

namespace shared::gamestate
{
	struct EditorComponent;
}

namespace editor::spellcraft
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::spellcraft::LinkCreateEvent,
			editor::spellcraft::LinkDestroyEvent,
			editor::spellcraft::NodeCreateEvent,
			editor::spellcraft::WindowComponent>
			::Read<
			editor::spellcraft::GraphComponent,
			editor::spellcraft::WindowEvent,
			shared::gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};