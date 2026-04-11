#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct VisibilityComponent;
	struct VisibilityTemplate;
}

namespace eng
{
	class VisibilitySystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::VisibilityComponent>
			::Read<
			eng::VisibilityTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
};