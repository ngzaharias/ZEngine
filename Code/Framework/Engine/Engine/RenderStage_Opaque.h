#pragma once

#include "Engine/RenderStage.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class AssetManager;
	class WindowManager;
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
	struct AmbientComponent;
	struct DirectionalComponent;
	struct PointComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
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
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::light::AmbientComponent,
			const eng::light::DirectionalComponent,
			const eng::light::PointComponent,
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
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}