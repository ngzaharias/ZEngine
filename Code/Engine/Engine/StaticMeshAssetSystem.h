#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct StaticMeshAssetComponent;
	struct StaticMeshComponent;
}

namespace eng
{
	class StaticMeshAssetSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::StaticMeshAssetComponent,
			const eng::StaticMeshComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}