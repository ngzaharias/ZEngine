#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct DynamicMeshComponent;
	struct TransformComponent;
}

namespace voxel
{
	struct ChunkChangedFrameComponent;
	struct ChunkComponent;
	struct ChunkLoadedFrameComponent;
	struct ModifyComponent;

	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::DynamicMeshComponent,
			const eng::TransformComponent,
			const voxel::ChunkChangedFrameComponent,
			const voxel::ChunkComponent,
			const voxel::ChunkLoadedFrameComponent,
			const voxel::ModifyComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}