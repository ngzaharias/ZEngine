#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct DynamicMeshComponent;
}

namespace shared::voxel
{
	struct ChunkComponent;
}

namespace client::voxel
{
	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::DynamicMeshComponent>
			::Read<
			shared::voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}