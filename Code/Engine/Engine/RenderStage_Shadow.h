#pragma once

#include "Engine/RenderStage.h"
#include "Engine/Texture2DAsset.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class AssetManager;

	class RenderStage_Shadow final : public RenderStage
	{
	public:
		RenderStage_Shadow(eng::AssetManager& assetManager);

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(ecs::EntityWorld& entityWorld, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData);

	private:
		uint32 m_ModelBuffer = 0;
	};
}