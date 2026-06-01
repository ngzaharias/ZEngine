#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct TransformComponent;
}

namespace shared::voxel
{
	struct ChunkComponent;
	struct ClientModifyEvent;
}

namespace server::voxel
{
	class ChunkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::voxel::ChunkComponent>
			::Read<
			eng::TransformComponent,
			shared::voxel::ClientModifyEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}