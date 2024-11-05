#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;
	struct SpriteComponent;
	struct StaticMeshComponent;
}

namespace eng
{
	class AssetLoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::FlipbookComponent,
			const eng::SpriteComponent,
			const eng::StaticMeshComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}