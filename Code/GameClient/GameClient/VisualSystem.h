#pragma once

#include "ECS/System.h"

namespace eng
{
	struct StaticMeshComponent;
}

namespace projectile
{
	struct ChangesComponent;
	struct CreateRequestComponent;
}

namespace visual
{
	/// \brief
	class VisualSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::StaticMeshComponent,
			const projectile::ChangesComponent,
			const projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}