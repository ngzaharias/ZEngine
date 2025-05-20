#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct TransformComponent;
	struct DynamicMeshComponent;
}

namespace voxel
{
	struct ChunkComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng
{
	class RenderStage_Voxels final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::WindowManager,
			// Components
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const eng::DynamicMeshComponent,
			const voxel::ChunkComponent>;

		void Initialise(ecs::EntityWorld& world) override;
		void Shutdown(ecs::EntityWorld& world) override;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}