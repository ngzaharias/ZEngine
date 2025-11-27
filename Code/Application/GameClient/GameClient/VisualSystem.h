#pragma once

#include "ECS/System.h"

namespace eng
{
	struct StaticMeshComponent;
}

namespace projectile
{
	struct ChangesSingleton;
	struct CreateRequestComponent;
}

namespace visual
{
	/// \brief
	class VisualSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			eng::StaticMeshComponent,
			const projectile::CreateRequestComponent,
			// Singletons
			const projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}