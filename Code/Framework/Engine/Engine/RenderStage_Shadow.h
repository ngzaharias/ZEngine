#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	struct FrameBufferSingleton;
	struct LightDirectionalComponent;
	struct StaticMeshComponent;
	struct TransformComponent;
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

namespace eng
{
	class RenderStage_Shadow final : public RenderStage
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			// Components
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::LightDirectionalComponent,
			const eng::StaticMeshComponent,
			const eng::TransformComponent,
			// Singletons
			eng::FrameBufferSingleton,
			const eng::settings::DebugSingleton>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(World& world, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData);

	private:
		uint32 m_ModelBuffer = 0;
	};
}