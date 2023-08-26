#pragma once

#include <ECS/Component.h>
#include <ECS/System.h>

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct ImGuiDemoRequestComponent;

	struct ImGuiDemoComponent : public ecs::SingletonComponent<ImGuiDemoComponent> { };

	class ImGuiDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::ImGuiDemoComponent,
			const dbg::ImGuiDemoRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};