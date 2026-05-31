#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct DynamicMeshComponent;
	struct TransformComponent;
}

namespace shared::voxel
{
	struct ChunkChangedComponent;
	struct ChunkComponent;
	struct ChunkLoadedComponent;
	struct ModifyComponent;
}

namespace shared::voxel
{
	class MeshingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::DynamicMeshComponent,
			shared::voxel::ChunkComponent>
			::Read<
			eng::TransformComponent,
			shared::voxel::ChunkChangedComponent,
			shared::voxel::ChunkLoadedComponent,
			shared::voxel::ModifyComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}