#pragma once

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace eng
{
	struct RigidStaticComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealedComponent;
}

namespace hidden
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::RigidStaticComponent,
			const hidden::ObjectComponent,
			const hidden::RevealedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}