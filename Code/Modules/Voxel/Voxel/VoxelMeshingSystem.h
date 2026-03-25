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
	struct ChunkChangedComponent;
	struct ChunkComponent;
	struct ChunkLoadedComponent;
	struct ModifyComponent;
}

namespace voxel
{
	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::DynamicMeshComponent,
			voxel::ChunkComponent>
			::Read<
			eng::TransformComponent,
			voxel::ChunkChangedComponent,
			voxel::ChunkLoadedComponent,
			voxel::ModifyComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}