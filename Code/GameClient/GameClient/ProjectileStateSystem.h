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

	/// \brief
	class StateSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			projectile::StateComponent,
			const projectile::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}