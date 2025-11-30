#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct ImGuiDemoRequest;

	struct ImGuiDemoWindowComponent final : public ecs::Component<ImGuiDemoWindowComponent> { };

	class ImGuiDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::ImGuiDemoWindowComponent,
			const dbg::ImGuiDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};