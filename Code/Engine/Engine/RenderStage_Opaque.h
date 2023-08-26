#pragma once

#include <Engine/RenderStage.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class AssetManager;

	class RenderStage_Opaque final : public eng::RenderStage
	{
	public:
		RenderStage_Opaque(eng::AssetManager& assetManager);
		~RenderStage_Opaque() override;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(ecs::EntityWorld& entityWorld, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData);

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}