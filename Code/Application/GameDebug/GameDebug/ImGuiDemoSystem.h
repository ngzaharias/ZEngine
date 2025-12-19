#pragma once

#include "ECS/System.h"

namespace debug
{
	struct ImGuiDemoRequest;
	struct ImGuiDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
}

namespace debug
{
	class ImGuiDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::ImGuiDemoWindowComponent,
			const debug::ImGuiDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};