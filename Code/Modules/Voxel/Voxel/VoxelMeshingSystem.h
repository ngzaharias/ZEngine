#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::DynamicMeshComponent>
			::Read<
			eng::TransformComponent,
			voxel::ChunkChangedFrameComponent,
			voxel::ChunkComponent,
			voxel::ChunkLoadedFrameComponent,
			voxel::ModifyComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}