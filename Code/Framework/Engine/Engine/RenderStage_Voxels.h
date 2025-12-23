#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct TransformComponent;
	struct DynamicMeshComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace voxel
{
	struct ChunkComponent;
}

namespace eng
{
	class RenderStage_Voxels final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::DynamicMeshComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			voxel::ChunkComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}