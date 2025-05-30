#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct FrameBufferComponent;
	struct LightAmbientComponent;
	struct LightDirectionalComponent;
	struct LightPointComponent;
	struct StaticMeshAssetComponent;
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
	struct DebugComponent;
}

namespace eng
{
	class RenderStage_Opaque final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::WindowManager,
			// Components
			eng::FrameBufferComponent,
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::LightAmbientComponent,
			const eng::LightDirectionalComponent,
			const eng::LightPointComponent,
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
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}