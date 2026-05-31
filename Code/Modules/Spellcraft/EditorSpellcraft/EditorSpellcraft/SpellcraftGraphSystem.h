#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::spell
{
	struct GraphComponent;
	struct LinkCreateEvent;
	struct LinkDestroyEvent;
	struct NodeCreateEvent;
	struct WindowComponent;
}

namespace editor::spell
{
	class GraphSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::spell::GraphComponent>
			::Read<
			editor::spell::LinkCreateEvent,
			editor::spell::LinkDestroyEvent,
			editor::spell::NodeCreateEvent,
			editor::spell::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};