#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct FlipbookAssetComponent;
	struct FlipbookComponent;
	struct SpriteAssetComponent;
	struct SpriteComponent;
	struct TransformComponent;
	struct VisibilityComponent;
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
	class RenderStage_Translucent final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::WindowManager,
			// Components
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::FlipbookAssetComponent,
			const eng::FlipbookComponent,
			const eng::settings::DebugComponent,
			const eng::SpriteAssetComponent,
			const eng::SpriteComponent,
			const eng::TransformComponent,
			const eng::VisibilityComponent>;

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