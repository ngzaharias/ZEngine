#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace projectile
{
	struct ChangesSingleton;
	struct CreateRequestComponent;
}

namespace movement
{
	struct AccelerationComponent;
	struct VelocityComponent;

	/// \brief
	class VelocitySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			movement::VelocityComponent>
			::Read<
			movement::AccelerationComponent,
			projectile::ChangesSingleton,
			projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}