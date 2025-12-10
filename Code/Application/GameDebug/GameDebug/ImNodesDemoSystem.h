#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	struct ImNodesDemoRequest;

	struct ImNodesDemoWindowComponent final : public ecs::Component<ImNodesDemoWindowComponent> { };

	class ImNodesDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::ImNodesDemoWindowComponent,
			const debug::ImNodesDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};