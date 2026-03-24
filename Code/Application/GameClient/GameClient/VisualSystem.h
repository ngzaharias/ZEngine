#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct StaticMeshComponent;
}

namespace projectile
{
	struct ChangesStaticComponent;
	struct CreateRequestComponent;
}

namespace visual
{
	/// \brief
	class VisualSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::StaticMeshComponent>
			::Read<
			projectile::ChangesStaticComponent,
			projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}