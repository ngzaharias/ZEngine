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

	class RenderStage_Voxels final : public eng::RenderStage
	{
	public:
		RenderStage_Voxels(eng::AssetManager& assetManager);

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}