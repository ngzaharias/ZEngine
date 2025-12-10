#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	struct ImGuiDemoRequest;

	struct ImGuiDemoWindowComponent final : public ecs::Component<ImGuiDemoWindowComponent> { };

	class ImGuiDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::ImGuiDemoWindowComponent,
			const debug::ImGuiDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};