#pragma once

#include "ECS/System.h"

namespace eng
{
	struct DynamicMeshComponent;
	struct TransformComponent;
}

namespace voxel
{
	struct ChunkComponent;
	struct ChunkChangedEventComponent;
	struct ChunkLoadedEventComponent;
	struct ModifyComponent;

	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::DynamicMeshComponent,
			const eng::TransformComponent,
			const voxel::ChunkComponent,
			const voxel::ChunkChangedEventComponent,
			const voxel::ChunkLoadedEventComponent,
			const voxel::ModifyComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}