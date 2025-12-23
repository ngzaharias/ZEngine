#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct ImGuiDemoRequest;
	struct ImGuiDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	class ImGuiDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::ImGuiDemoWindowComponent>
			::Read<
			debug::ImGuiDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};