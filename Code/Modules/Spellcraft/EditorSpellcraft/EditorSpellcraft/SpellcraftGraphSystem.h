#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::spellcraft
{
	struct GraphComponent;
	struct LinkCreateEvent;
	struct LinkDestroyEvent;
	struct NodeCreateEvent;
	struct WindowComponent;
}

namespace editor::spellcraft
{
	class GraphSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::spellcraft::GraphComponent>
			::Read<
			editor::spellcraft::LinkCreateEvent,
			editor::spellcraft::LinkDestroyEvent,
			editor::spellcraft::NodeCreateEvent,
			editor::spellcraft::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};