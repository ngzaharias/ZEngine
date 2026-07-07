#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace render
{
	struct DockspaceComponent;
}

namespace render
{
	class DockspaceSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			render::DockspaceComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}