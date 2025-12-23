#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::AssetComponent,
			eng::AssetManager>
			::Read<
			eng::AssetLoadedEvent,
			eng::FlipbookComponent,
			eng::SpriteComponent,
			eng::StaticMeshComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};