#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct FlipbookComponent;
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
	struct DebugSingleton;
}

namespace eng
{
	class RenderStage_Translucent final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::FlipbookComponent,
			eng::settings::DebugSingleton,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent,
			eng::WindowManager>;

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