#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct StaticMeshComponent;
}

namespace eng
{
	class StaticMeshSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::StaticMeshComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}