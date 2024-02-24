#pragma once

#include <ECS/System.h>

namespace eng
{
	struct TransformComponent;
}

namespace spring
{
	struct DamperComponent;
}

namespace spring
{
	/// \brief
	class DamperSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			spring::DamperComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}