#pragma once

#include <ECS/System.h>

namespace movement
{
	struct AccelerationComponent;
	struct VelocityComponent;

	class VelocitySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			movement::VelocityComponent,
			const movement::AccelerationComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}