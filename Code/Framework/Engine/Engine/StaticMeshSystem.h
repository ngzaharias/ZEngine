#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct StaticMeshComponent;
	struct StaticMeshTemplate;
}

namespace eng
{
	class StaticMeshSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::StaticMeshComponent>
			::Read<
			eng::StaticMeshTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
};