#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct PhysicsComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace hidden
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::PhysicsComponent,
			const hidden::ObjectComponent,
			const hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}