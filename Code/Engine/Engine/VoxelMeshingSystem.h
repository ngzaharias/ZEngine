#pragma once

#include "ECS/System.h"

namespace eng
{
	struct DynamicMeshComponent;
}

namespace voxel
{
	struct ChunkComponent;
	struct ChunkChangedEventComponent;
	struct ChunkLoadedEventComponent;

	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::DynamicMeshComponent,
			const voxel::ChunkComponent,
			const voxel::ChunkChangedEventComponent,
			const voxel::ChunkLoadedEventComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}