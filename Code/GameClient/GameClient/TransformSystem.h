#pragma once

#include <ECS/System.h>

namespace eng
{
	struct TransformComponent;
}

namespace projectile
{
	struct StateComponent;
	struct SettingsComponent;
}

namespace transform
{
	/// \brief
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			const projectile::StateComponent,
			const projectile::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}