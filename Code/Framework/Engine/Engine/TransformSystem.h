#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct TransformComponent;
	struct TransformTemplate;
}

namespace eng
{
	/// \brief
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			eng::TransformTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}