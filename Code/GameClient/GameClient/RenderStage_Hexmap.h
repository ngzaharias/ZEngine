#pragma once


#include "Engine/RenderStage.h"

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng
{
	class AssetManager;
	struct TransformComponent;
}

namespace hexmap
{
	struct AssetComponent;
	struct FragmentComponent;

	class RenderStage final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const hexmap::AssetComponent,
			const hexmap::FragmentComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		void RenderBatch(World& world, const eng::RenderBatchID& batchID, const eng::RenderBatchData& batchData, const eng::RenderStageData& stageData);

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}