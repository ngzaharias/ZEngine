#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct ModelGenWindowComponent;
	struct ModelGenWindowEvent;
}

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	class ModelGenSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::ModelGenWindowComponent>
			::Read<
			debug::ModelGenWindowEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
};