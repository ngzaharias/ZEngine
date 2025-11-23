#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct AssetComponent;
	struct AssetLoadedEvent;
	struct FlipbookComponent;
	struct SpriteComponent;
	struct StaticMeshComponent;
}

namespace eng
{
	class AssetSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager, 
			// Components
			eng::AssetComponent, 
			const eng::FlipbookComponent, 
			const eng::SpriteComponent, 
			const eng::StaticMeshComponent, 
			// Events
			const eng::AssetLoadedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
};