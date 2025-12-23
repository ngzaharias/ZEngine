#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	struct FrameBufferSingleton;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::light
{
	struct DirectionalComponent;
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
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::FrameBufferSingleton>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::light::DirectionalComponent,
			eng::settings::DebugSingleton,
			eng::StaticMeshComponent,
			eng::TransformComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(World& world, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData);

	private:
		uint32 m_ModelBuffer = 0;
	};
}