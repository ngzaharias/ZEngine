#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace voxel
{
	struct ChunkComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng
{
	class AssetManager;
	struct TransformComponent;
	struct DynamicMeshComponent;

	class RenderStage_Voxels final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const eng::DynamicMeshComponent,
			const voxel::ChunkComponent>;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}