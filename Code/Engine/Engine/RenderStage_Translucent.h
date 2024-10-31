#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;
	struct SpriteComponent;
	struct TransformComponent;

	class RenderStage_Translucent final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::camera::ProjectionComponent,
			const eng::FlipbookComponent,
			const eng::SpriteComponent,
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