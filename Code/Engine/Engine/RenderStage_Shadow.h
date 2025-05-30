#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace eng
{
	class AssetManager;
	struct FrameBufferComponent;
	struct LightDirectionalComponent;
	struct StaticMeshAssetComponent;
	struct StaticMeshComponent;
	struct TransformComponent;

	class RenderStage_Shadow final : public RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::FrameBufferComponent,
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::LightDirectionalComponent,
			const eng::settings::DebugComponent,
			const eng::StaticMeshAssetComponent,
			const eng::StaticMeshComponent,
			const eng::TransformComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(World& world, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData);

	private:
		uint32 m_ModelBuffer = 0;
	};
}